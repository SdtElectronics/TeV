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

    virtual void onWriteError  (TCPsession& session, const std::string& err)override{
        std::cout << err << std::endl;
    }

    virtual void onReadError   (TCPsession& session, const std::string& err)override{
        std::cout << err << std::endl;
    }

    virtual void onConnectError(TCPsession& session, const std::string& err)override{
        std::cout << err << std::endl;
    }

    virtual ~TimeoutServer(){
        std::cout << "destructed" << std::endl;
    }

  private:
    void doRead(TCPsession& session){
        auto timerPtr = std::make_shared<TCPsession::Timer>(
            session.expire(std::chrono::seconds(10), 
            [](const std::error_code& ec){
                std::cout << "No message from client after 10s, session expired" << std::endl;
            })
        );

        session.asyncRead('\0', 
        [this, timerPtr](TCPsession& session_, const std::string& msg){
            std::cout << msg << std::endl;
            timerPtr->cancel();
            doRead(session_);
        });
    }
};

int main(int argc, char* argv[]){
    asio::io_context io_context;
    Service<> srv(atoi(argv[1]), io_context);
    srv.start([]{return std::make_shared<TimeoutServer>();});
    io_context.run();
}