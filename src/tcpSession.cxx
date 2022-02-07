// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef TEVHDRONLY
#include <limits>

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
    std::istream stream(&message_);
    std::string result(length, ' ');
    stream.read(&result[0], length);
    return result;
}

std::string TCPsession::getMessage(){
    std::istream stream(&message_);
    std::string ret(std::istreambuf_iterator<char>(stream), {});
    return ret;
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