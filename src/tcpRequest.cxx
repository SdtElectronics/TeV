// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef TEVHDRONLY
#include "tcp/tcpRequest.h"
#endif

#include "worker.h"

namespace tev{

TCPrequest::TCPrequest(dialoguePtr dialogue , std::size_t maxLength):
    TCPsession(tcp::socket(Worker::getContext()), dialogue, maxLength),
    resolver_(Worker::getContext()){
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
    asio::async_connect(stream_, results, 
    [self](const std::error_code& ec, const tcp::endpoint&){
        if(!ec){
            self->dialogue_->onConnect(*self);
        }else{
            self->dialogue_->onConnectError(*self, ec.message());
        }
    });
}

}