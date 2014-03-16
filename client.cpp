#include "client.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "string.h"
#include "assert.h"
#include "unistd.h"
#include <regex>
#include <iostream>
#include <thread>

client::client()
: socketfd(-1)
, bShouldSendCommands(true)
{
    handlers.push_back(onAllDebug);
    handlers.push_back(onPing);
    handlers.push_back(onEndMotd);
    connect();

    recvThread = std::thread(&client::readLoop, this);

    std::string a;
    a += ("NICK strup\r\n");
    a += ("user mark pumpingstationone.org chat.freenode.net :mark\r\n");
    int length = ::send(socketfd, a.c_str(), a.length(), 0);
    assert( a.length() == length );
}

client::~client()
{
    sendThread.join();
    recvThread.join();
}

void client::connect()
{
    struct addrinfo hints;
    struct addrinfo* serverinfo;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int res = getaddrinfo(
            "chat.freenode.net",
            "6667",
            &hints,
            &serverinfo);
    (void)res;
    assert(res == 0);

    (void)socketfd;

    for(addrinfo* p = serverinfo; p != NULL; p = p->ai_next)
    {
        socketfd = socket(
                p->ai_family,
                p->ai_socktype,
                p->ai_protocol);
        if( socketfd == -1)
        {
            std::cout << "Failed to build socketfd" << std::endl;
            continue;
        }

        res = ::connect(socketfd, p->ai_addr, p->ai_addrlen);
        if(res == -1)
        {
            std::cout << "Failed to connect" << std::endl;
            close(socketfd);
            continue;
        }
        break;
    }
    assert(res == 0);
    freeaddrinfo(serverinfo);
}

void client::readLoop()
{
    //struct timeval timeout;
    //timeout.tv_sec = 30;
    //timeout.tv_usec = 0;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(socketfd, &readfds);

    bool running = true;
    int result = -1;
    while( running )
    {
        result = select(socketfd+1, &readfds, nullptr, nullptr, nullptr);
        if( result == -1 )
        {
            std::cout << "selecterr: " <<  strerror(errno) << std::endl;
            running = false;
            break;
        }
        
        char buffer[2];
        buffer[1] = '\0';
        int res = recv(socketfd, buffer, 1, 0);

        if(res == -1)
        {
            std::cout << "recverr: " <<  strerror(errno) << std::endl;
            running = false;
            break;
        }

        if(res == 0)
        {
            std::cout << "recv: socket closed" << std::endl;
            running = false;
            break;
        }

        bool cr=false;
        switch(buffer[0])
        {
            case '\r':
            {
                if(cr)
                    server_message += '\r';
                cr = true;
            }
            break;
            case '\n':
            {
                if(cr || true)
                {
                    for(auto handler: handlers)
                    {
                        handler(*this, server_message);
                    }
                    server_message.clear();
                }
                else
                {
                    server_message += '\n';
                }
            }
            break;
            default:
            {
                if(cr)
                    server_message += '\r';
                cr = false;
                server_message += buffer[0];
            }
        }
    }
}

void client::sendRaw(std::string message)
{
    std::cout << "send: " << message << std::endl;
    message += "\r\n";
    int sent = 0;
    int result = -1;
    do {
        result = ::send(socketfd, message.c_str() + sent, message.length() - sent, 0);
        sent += result;
    } while( sent < (int)message.length() && result != -1);

    if(result == -1)
    {
        std::cout << "senderr: " <<  strerror(errno) << std::endl;
    }
}

void client::commandLoop()
{
    while(bShouldSendCommands)
    {
        command* c = commands.waitPop();
        if(c)
        {
            c->operator()(*this);
        }
    }
}

void client::send(std::string message)
{
    command* c = new rawMessage(message);
    commands.push(c);
}

void client::quit(std::string quitMessage)
{
    command* c = new quitCommand(quitMessage);
    commands.push(c);
}

void client::registerHandler( std::function<void(client&, std::string)> handler )
{
    handlers.push_back(handler);
}

void client::onAllDebug(client&, std::string message)
{
    std::cout << "recv: " << message << std::endl;
}

void client::onPing(client& c, std::string message)
{
    std::smatch match;
    std::regex e("PING :(.*)");
    std::regex_match(message, match, e);

    if(match.size())
    {
        std::string pong("PONG :");
        pong += match[1];
        c.send(pong);
    }
}

void client::onEndMotd(client& c, std::string message)
{
    std::regex expression(R"(\S+ 376 .*)");
    if(std::regex_match(message, expression))
    {
        c.sendThread = std::thread(&client::commandLoop, &c);
    }
    // todo unregister
}

void client::rawMessage::operator()(client& c)
{
    c.sendRaw(message);
}

void client::quitCommand::operator()(client& c)
{
    quitMessage = "QUIT :" + quitMessage;
    c.bShouldSendCommands = false;
    c.sendRaw(quitMessage);
    close(c.socketfd);
}

int not_main( int argc, char* argv[] )
{
    (void)argc;
    (void)argv;
    client c;
    c.send("JOIN #pumpingstationone");
    std::chrono::milliseconds duration( 30000 );
    std::chrono::milliseconds duration2( 100 );
    std::this_thread::sleep_for(duration);
    //c.send("QUIT :Farwell, cruel world");
    c.quit();
    std::this_thread::sleep_for(duration2);
    std::cout << "leaving main" << std::endl;
    return 0;
}
