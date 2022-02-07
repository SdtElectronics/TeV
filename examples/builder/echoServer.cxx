#include <iostream>
#include "sessionBuilder.h"
#include "sharedBuf.h"
#include "tcp/tcpService.h"

using namespace tev;

void doRead(TCPsession& session){
    // Message is nul terminated
    session.asyncRead('\0', [](TCPsession& session_){
        // Send back the message as-is
        session_.asyncWrite(SharedBuffer(session_.getMessage()), 
        // and repeat (note no recursion is incurred here)
        [](TCPsession& session__){
            doRead(session__);
        });
    });
}

// First command line parameter is the port to listen
int main(int argc, char* argv[]){
    asio::io_context io_context;
    
    auto session = SessionBuilder<>().onConnect([](TCPsession& session){
        doRead(session);
    }).onDisconnect([](TCPsession& session){
        std::cout << session.getRemoteIP() << ':' << session.getRemotePort() 
                  << " disconnected" << std::endl;
    }).onError([](TCPsession& session, const std::string& err, ErrorType){
        std::cout << err << std::endl;
    }).buildFactory();

    Service<> srv(atoi(argv[1]), io_context);
    srv.start(session);
    io_context.run();
}