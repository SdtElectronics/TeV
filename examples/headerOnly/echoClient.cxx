#include <iostream>

#include "tev.h"

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
        buf += '\0';
        session.asyncWrite(SharedBuffer(std::move(buf)), [this](TCPsession& session_){
            session_.asyncRead('\0', [this](TCPsession& session__){
                std::cout << session__.getMessage() << std::endl;
                doWrite(session__);
            });
        });
    }

  private:
};

int main(int argc, char* argv[]){
    std::make_shared<TCPrequest>(std::make_shared<EchoClient>())->send(argv[1], argv[2]);
    Worker::run();
}