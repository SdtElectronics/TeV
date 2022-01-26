#include "tcp/tcpRequest.h"

namespace tev{

TCPrequest::TCPrequest(dialoguePtr dialogue, asio::io_context& asio_ctx, std::size_t maxLength):
    TCPsession(tcp::socket(asio_ctx), dialogue, maxLength),
    resolver_(asio_ctx),
    asio_ctx_(asio_ctx){
}

void TCPrequest::send(const char* hostname, const char* port){
    auto self(this->shared_from_this());
    resolver_.async_resolve(hostname, port, 
    [this, self](
        const std::error_code& ec, const tcp::resolver::results_type& results){
        if (!ec){
            asyncConnect(results);
        }else{
            dialogue_->onConnectError(*this, ec.message());
        }
    });
}

void TCPrequest::asyncConnect(const tcp::resolver::results_type& results){
    auto self(this->shared_from_this());
    asio::async_connect(socket_, results, 
    [this, self](const std::error_code& ec, const tcp::endpoint& endpoint){
        if(!ec){
            dialogue_->onConnect(*this);
        }else{
            dialogue_->onConnectError(*this, ec.message());
        }
    });
}

}