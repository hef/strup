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
{
    handlers.push_back(onAllDebug);
    handlers.push_back(onPing);
    handlers.push_back(onEndMotd);
    connect();
//    send("NICK mark_3519");
//    send("user mark pumpingstationone.org chat.freenode.net :mark");

    std::string a("NICK mark_3519\r\n");
    std::string b("user mark pumpingstationone.org chat.freenode.net :mark\r\n");
    int length = ::send(socketfd, a.c_str(), a.length(), 0);
    assert( a.length() == length );
    length = ::send(socketfd, b.c_str(), b.length(), 0);
    assert( b.length() == length );

}

client::~client()
{
    close(socketfd);
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
            std::cerr << "Failed to build socketfd" << std::endl;
            continue;
        }

        res = ::connect(socketfd, p->ai_addr, p->ai_addrlen);
        if(res == -1)
        {
            std::cerr << "Failed to connect" << std::endl;
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
    struct timeval timeout;
    timeout.tv_sec = 30;
    timeout.tv_usec = 0;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(socketfd, &readfds);

    bool running = true;
    while( running )
    {
        select(socketfd+1, &readfds, nullptr, nullptr, &timeout);
        
        char buffer[2];
        buffer[1] = '\0';
        int res = recv(socketfd, buffer, 1, 0);

        if(res == -1)
        {
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

void client::writeLoop()
{
    while(true)
    {
        command* c = commands.waitPop();
        if(c)
        {
            std::cout << "send: " << c->message << std::endl;
            int length = ::send(socketfd, c->message.c_str(), c->message.length(), 0);
            assert( c->message.length() == length );
            length = ::send(socketfd, "\r\n", 2, 0);
            assert( 2 == length );
        }
    }
}

void client::send(std::string message)
{
    command* c = new command(message);
    commands.push(c);
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

    (void)c;
    (void)message;
    std::regex expression(R"(\S+ 376 .*)");
    if(std::regex_match(message, expression))
    {
        c.send_thread = std::thread(&client::writeLoop, &c);
    }
}


int main( int argc, char* argv[] )
{
    (void)argc;
    (void)argv;
    client c;
    std::thread recv_thread(&client::readLoop, &c);
    c.send("JOIN #pumpingstationone");
    std::chrono::milliseconds duration2( 30000 );
    std::this_thread::sleep_for(duration2);
    return 0;
}
