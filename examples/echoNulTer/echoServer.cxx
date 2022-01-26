
#include <iostream>
#include "session.h"
#include "tcp/tcpService.h"

using namespace tev;

class EchoServer: public Session{
  public:
    virtual void onConnect     (TCPsession& session                        )override{
        doRead(session);
    }

    virtual void onDisconnect  (TCPsession& session                        )override{
        std::cout << session.getRemoteIP() << ':' << session.getRemotePort() 
                  << " disconnected" << std::endl;
    }

    virtual void onWriteError  (TCPsession& session, const std::string& err)override{
        std::cout << err << std::endl;
    }

    virtual void onReadError   (TCPsession& session, const std::string& err)override{
        std::cout << err << std::endl;
    }

    virtual void onConnectError(TCPsession& session, const std::string& err)override{
        std::cout << err << std::endl;
    }

  private:
    void doRead(TCPsession& session){
        session.asyncRead('\0', [this](TCPsession& session_, const std::string& msg){
            std::cout << msg << std::endl;
            session_.asyncWrite(msg, [this](TCPsession& session__){
                doRead(session__);
            });
        });
    }
};

int main(int argc, char* argv[]){
    asio::io_context io_context;
    Service<> srv(atoi(argv[1]), io_context);
    srv.start([]{return std::make_shared<EchoServer>();});
    io_context.run();
}