#ifndef CLIENT_H
#define CLIENT_H
#include "slist.h"
#include <vector>
#include <functional>
#include <string>
#include <thread>
class client
{
public:
    client();
    ~client();
private:
    void connect();
    void writeLoop();
    std::thread send_thread;
public:
    void readLoop();
    void send(std::string message);
private:
    static void onAllDebug(client&, std::string);
    static void onEndMotd(client&, std::string);
    static void onPing(client&, std::string);

    struct command
    {
        command(std::string message) : message(message), slist_next(nullptr){}
        std::string message;
        command* slist_next;
    };
    
    slist<command> commands;
    int socketfd;
    std::vector<std::function<void(client&,  std::string)>> handlers;
    std::string server_message;
};
#endif // ifndef CLIENT_H
