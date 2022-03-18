#include <iostream>

#include "sessionBuilder.h"
#include "tcp/tcpRequest.h"

using namespace tev;

void doWrite(TCPsession& session){
    std::string buf;
    std::getline(std::cin, buf);
    buf += '\0';
    // buf is a temporary object, wrapping with SharedBuffer
    // to extend its lifetime till execution of callback
    session.asyncWrite(SharedBuffer(std::move(buf)), [](TCPsession& session_){
        session_.asyncRead('\0', [](TCPsession& session__){
            std::cout << session__.getMessage() << std::endl;
            doWrite(session__);
        });
    });
}

int main(int argc, char* argv[]){
    // <> after SessionBuilder can be omitted for C++17 or above
    auto session = SessionBuilder<>().onConnect([](TCPsession& session){
        doWrite(session);
    }).onDisconnect([](TCPsession& session){
        std::cout << session.getRemoteIP() << ':' << session.getRemotePort() 
                  << " disconnected" << std::endl;
    }).onError([](TCPsession& session, const std::string& err, ErrorType){
        std::cout << err << std::endl;
    }).build();

    std::make_shared<TCPrequest>(session)->send(argv[1], argv[2]);
    Worker::run();
}