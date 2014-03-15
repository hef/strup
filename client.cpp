#include "client.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "string.h"
#include "assert.h"
#include "unistd.h"
#include <regex>
#include <iostream>

client::client()
: socketfd(-1)
{
    handlers.push_back(onAllDebug);
    connect();
    send("NICK mark_3519");
    send("user mark pumpingstationone.org chat.freenode.net :mark");
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

void client::loop()
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

void client::send(std::string message)
{
    command* c = new command(message);
    commands.push(c);
}

void client::onPing(const client&, std::string)
{
}

void client::onAllDebug(const client&, std::string message)
{
    std::cout << message << std::endl;
}

int main( int argc, char* argv[] )
{
    (void)argc;
    (void)argv;
    client c;
    c.loop();
    return 0;
}
