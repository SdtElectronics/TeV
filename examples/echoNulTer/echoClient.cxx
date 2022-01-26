#include <iostream>

#include "session.h"
#include "tcp/tcpRequest.h"

using namespace tev;

class EchoClient: public Session{
  public:
    virtual void onConnect     (TCPsession& session                        )override{
        doWrite(session);
    }

    virtual void onDisconnect  (TCPsession& session                        )override{
        std::cout << session.getRemoteIP() << ':' << session.getRemotePort() 
                  << " disconnected" << std::endl;
    }

    virtual void onConnectError(TCPsession& session, const std::string& err)override{
        std::cout << err << std::endl;
    }

    virtual void onReadError   (TCPsession& session, const std::string& err)override{
        std::cout << err << std::endl;
    }

    virtual void onWriteError  (TCPsession& session, const std::string& err)override{
        std::cout << err << std::endl;
    }

    void doWrite(TCPsession& session){
        std::string buf;
        std::getline(std::cin, buf);
        buf += '\0';
        session.asyncWrite(buf, [this](TCPsession& session_){
            session_.asyncRead('\0', [this](TCPsession& session__, const std::string& msg){
                std::cout << msg << std::endl;
                doWrite(session__);
            });
        });
    }

  private:
};

int main(int argc, char* argv[]){
    asio::io_context io_context;
    std::make_shared<TCPrequest>(
        std::make_shared<EchoClient>(),
        io_context
    )->send(argv[1], argv[2]);
    io_context.run();
}