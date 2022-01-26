#ifndef TEVHDRONLY
#include "bufReader.h"
#include "tcp/tcpSession.h"
#endif

namespace tev{

TCPsession::TCPsession (tcp::socket socket, 
                        std::shared_ptr<Session> dialogue, 
                        std::size_t maxLength):
    socket_(std::move(socket)), 
    dialogue_(dialogue),
    message_(maxLength){
}

void TCPsession::close(){
    std::error_code ec;
    socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
    socket_.close();
}

std::string TCPsession::getMessage(std::size_t length){
    return BufReader(message_)(length);
}

std::string TCPsession::getMessage(){
    return BufReader(message_)();
}

std::string TCPsession::getRemoteIP() const{
    return socket_.remote_endpoint().address().to_string();
}

unsigned short TCPsession::getRemotePort() const{
    return socket_.remote_endpoint().port();
}

void TCPsession::handleReadError(const std::error_code& ec){
    if(ec == asio::error::eof || ec == asio::error::connection_reset){
        dialogue_->onDisconnect(*this);
    }else{
        dialogue_->onReadError(*this, ec.message());
    }
}

}