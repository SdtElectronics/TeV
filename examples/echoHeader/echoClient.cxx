#include <iostream>

#include "session.h"
#include "tcp/tcpRequest.h"
#include "../hdrProtocol/hdrProtocol.h"

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

    virtual void onError(TCPsession& session, const std::string& err, ErrorType)override{
        std::cout << err << std::endl;
    }

    void doWrite(TCPsession& session){
        std::string buf;
        std::getline(std::cin, buf);
        headerProtocol::send(session, buf, [this](TCPsession& session_){
            headerProtocol::receive(session_, 
            [this](TCPsession& session__){
                std::cout << session__.getMessage() << std::endl;
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