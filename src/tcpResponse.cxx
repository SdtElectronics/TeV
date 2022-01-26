#ifndef TEVHDRONLY
#include "internal/tcpResponse.h"
#endif

namespace tev{
namespace internal{

TCPresponse::TCPresponse(tcp::socket socket, 
                         std::shared_ptr<Session> dialogue, 
                         std::size_t maxLength):
    TCPsession(std::move(socket), dialogue, maxLength){
}

void TCPresponse::start(){
    dialogue_->onConnect(*this);
}

}
}