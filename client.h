#ifndef CLIENT_H
#define CLIENT_H
#include "slist.h"
#include <vector>
#include <functional>
#include <string>
class client
{
public:
    client();
    ~client();
private:
    void connect();
public:
    void loop();
    void send(std::string message);
private:
    static void onPing(const client&, std::string);
    static void onAllDebug(const client&, std::string);

    struct command
    {
        command(std::string message) : message(message), slist_next(nullptr){}
        std::string message;
        command* slist_next;
    };
    
    slist<command> commands;
    int socketfd;
    std::vector<std::function<void(const client&,  std::string)>> handlers;
    std::string server_message;
};
#endif // ifndef CLIENT_H
