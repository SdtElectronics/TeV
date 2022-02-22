#include <iostream>
#include <sstream>

#include "session.h"
#include "tcp/tcpRequest.h"

using namespace tev;

class HTTPclient: public Session{
  public:
    HTTPclient(const char* remote, const char* path){
        buf_ << "GET " << path << " HTTP/1.0\r\n";
        buf_ << "Host: " << remote << "\r\n";
        buf_ << "Accept: */*\r\n";
        buf_ << "Connection: close\r\n\r\n";
    }

    virtual void onConnect     (TCPsession& session                        )override{
        auto timerPtr = std::make_shared<TCPsession::Timer>(
            session.expire(std::chrono::seconds(60), 
            [](const std::error_code& ec){
                std::cout << "Request timeout" << std::endl;
            })
        );

        session.asyncWrite(buf_, [this, timerPtr](TCPsession& session_){
            session_.asyncRead("\r\n\r\n", [this, timerPtr](TCPsession& session__){
                timerPtr->cancel();
                std::cout << session__.getMessage() << std::endl;
                session__.close();
            });
        });
    }

    virtual void onDisconnect  (TCPsession& session                        )override{
        std::cout << session.getRemoteIP() << ':' << session.getRemotePort() 
                  << " disconnected" << std::endl;
    }

    virtual void onError(TCPsession& session, const std::string& err, ErrorType)override{
        std::cout << err << std::endl;
    }

  private:
    SharedBuffer buf_;
};

int main(int argc, char* argv[]){
    asio::io_context io_context;
    std::make_shared<TCPrequest>(
        std::make_shared<HTTPclient>(argv[1], argv[2]),
        io_context
    )->send(argv[1], "http", std::chrono::seconds(180));
    io_context.run();
}