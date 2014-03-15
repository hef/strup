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
    connect();
//    send("NICK mark_3519");
//    send("user mark pumpingstationone.org chat.freenode.net :mark");
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

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int res = getaddrinfo(
            "irc.freenode.net",
//            "acm.cs.uic.edu",
            "6667",
            &hints,
            &serverinfo);
    (void)res;
    assert(res == 0);
    std::cout << "gotaddr" << std::endl;

    (void)socketfd;

    for(addrinfo* p = serverinfo; p != NULL; p = p->ai_next)
    {
        std::cout << "trying" << std::endl;
        socketfd = socket(
                p->ai_family,
                p->ai_socktype,
                p->ai_protocol);
        if( socketfd == -1)
        {
            std::cerr << "Failed to build socketfd" << std::endl;
            continue;
        }

        std::cout << "connecting" << std::endl;
        res = ::connect(socketfd, p->ai_addr, p->ai_addrlen);
        //res = ::connect(socketfd, serverinfo->ai_addr, serverinfo->ai_addrlen);
        std::cout << "connected" << std::endl;
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
    std::cout << "done connecting" << std::endl;
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
            delete c;
        }
    }
}

void client::send(std::string message)
{
    std::cout << "pushing" << message << std::endl;
    command* c = new command(message);
    commands.push(c);
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

void client::onAllDebug(client&, std::string message)
{
    std::cout << "recv: " << message << std::endl;
}

int main( int argc, char* argv[] )
{
    (void)argc;
    (void)argv;
    client c;
    std::thread send_thread(&client::writeLoop, &c);
    std::thread recv_thread(&client::readLoop, &c);
    std::chrono::milliseconds duration( 2000 );
    std::this_thread::sleep_for(duration);
    std::cout << "signing on" << std::endl;
    c.send("NICK mark_3519");
    c.send("user mark pumpingstationone.org chat.freenode.net :mark");
    std::chrono::milliseconds duration2( 30000 );
    std::this_thread::sleep_for(duration2);
    return 0;
}
