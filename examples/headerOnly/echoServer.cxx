
#include <iostream>

#include "tev.h"

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

    virtual void onError(TCPsession& session, const std::string& err, ErrorType)override{
        std::cout << err << std::endl;
    }

  private:
    void doRead(TCPsession& session){
        session.asyncRead('\0', [this](TCPsession& session_){
            session_.asyncWrite(SharedBuffer(session_.getMessage()), 
            [this](TCPsession& session__){
                doRead(session__);
            });
        });
    }
};

int main(int argc, char* argv[]){
    Service<> srv(atoi(argv[1]));
    srv.start([]{return std::make_shared<EchoServer>();});
    Worker::run();
}