// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef TEVHDRONLY
#include "tcp/tcpRequest.h"
#endif

namespace tev{

TCPrequest::TCPrequest(dialoguePtr dialogue, asio::io_context& asio_ctx, std::size_t maxLength):
    TCPsession(tcp::socket(asio_ctx), dialogue, maxLength),
    resolver_(asio_ctx),
    asio_ctx_(asio_ctx){
}

void TCPrequest::send(const char* hostname, const char* port){
    auto self(std::static_pointer_cast<TCPrequest>(this->shared_from_this()));
    resolver_.async_resolve(hostname, port, 
    [self](const std::error_code& ec, const tcp::resolver::results_type& results){
        if (!ec){
            self->asyncConnect(results);
        }else{
            self->dialogue_->onConnectError(*self, ec.message());
        }
    });
}

void TCPrequest::asyncConnect(const tcp::resolver::results_type& results){
    auto self(std::static_pointer_cast<TCPrequest>(this->shared_from_this()));
    asio::async_connect(socket_, results, 
    [self](const std::error_code& ec, const tcp::endpoint&){
        if(!ec){
            self->dialogue_->onConnect(*self);
        }else{
            self->dialogue_->onConnectError(*self, ec.message());
        }
    });
}

}