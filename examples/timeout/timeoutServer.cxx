#include <iostream>
#include "session.h"
#include "tcp/tcpService.h"

using namespace tev;

class TimeoutServer: public Session{
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

    virtual ~TimeoutServer(){
    }

  private:
    void doRead(TCPsession& session){
        auto timerPtr = std::make_shared<Timer>(
            session.expire(std::chrono::seconds(10), 
            [](const std::error_code& ec){
                std::cout << "No message from client after 10s, session expired" << std::endl;
            })
        );

        session.asyncRead('\0', 
        [this, timerPtr](TCPsession& session_){
            std::cout << session_.getMessage() << std::endl;
            timerPtr->cancel();
            doRead(session_);
        });
    }
};

int main(int argc, char* argv[]){
    Service<> srv(atoi(argv[1]));
    srv.start([]{return std::make_shared<TimeoutServer>();});
    Worker::run();
}