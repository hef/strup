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
    void sendRaw(std::string message);
    void commandLoop();
    void readLoop();
    std::thread sendThread;
    std::thread recvThread;
public:
    void send(std::string message);
    void quit(std::string message = "Farewell, Cruel World.");
    void registerHandler(std::function<void(client&, std::string)> handler);
private:
    static void onAllDebug(client&, std::string);
    static void onEndMotd(client&, std::string);
    static void onPing(client&, std::string);

    struct command
    {
        command() : slist_next(nullptr){}
        virtual ~command(){}
        virtual void operator()(client&) = 0;
        command* slist_next;
    };

    struct rawMessage: public command
    {
        rawMessage(std::string message) : command(), message(message){}
        virtual void operator()(client&);
        std::string message;
    };

    struct quitCommand: public command
    {
        quitCommand(std::string quitMessage): command(), quitMessage(quitMessage){}
        virtual void operator()(client&);
        std::string quitMessage;
    };
    
    slist<command> commands;
    int socketfd;
    std::vector<std::function<void(client&,  std::string)>> handlers;
    std::string server_message;
    std::atomic<bool> bShouldSendCommands;
};
#endif // ifndef CLIENT_H
