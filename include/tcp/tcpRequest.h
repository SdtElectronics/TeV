// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef tevtcprequest
#define tevtcprequest

#include <memory>
#include <utility>

#include "asio/asio.hpp"
#include "internal/tevdefs.h"
#include "session.h"
#include "tcpSession.h"

namespace tev{

using asio::ip::tcp;

class TCPrequest: public TCPsession{
  public:
    using dialoguePtr  = std::shared_ptr<Session>;
    // dialogue: std::shared_ptr to the request object
    TEVINLINE
    TCPrequest(dialoguePtr dialogue , std::size_t maxLength = 65535);
    // send request to hostname:port
    TEVINLINE void send(const char* hostname, const char* port);

    template<typename Rep, typename Period>
    using duration = std::chrono::duration<Rep, Period>;
    // send request to hostname:port, cancel if failed to connect within timeout
    template<typename Rep, typename Period>
    void send(const char* hostname, const char* port, const duration<Rep, Period>& timeout); 

  private:
    TEVINLINE void asyncConnect(const tcp::resolver::results_type& results);

    tcp::resolver resolver_;
};

template<typename Rep, typename Period>
void TCPrequest::send(const char* hostname, const char* port, const duration<Rep, Period>& timeout){
    auto self(std::static_pointer_cast<TCPrequest>(this->shared_from_this()));
    auto timerPtr = std::make_shared<Timer>(
        expire(timeout, 
        [this](const std::error_code& ec){
            dialogue_->onConnectError(*this, std::string("Resolve timeout"));
        })
    );

    resolver_.async_resolve(hostname, port, 
    [self, timerPtr](const std::error_code& ec, const tcp::resolver::results_type& results) mutable{
        timerPtr->cancel();
        timerPtr = std::make_shared<Timer>(
            self->expireAt(timerPtr->expiry(), 
            [self](const std::error_code& ec){
                self->dialogue_->onConnectError(*self, std::string("Connect timeout"));
            })
        );

        if (!ec){
            asio::async_connect(self->stream_, results, 
            [self, timerPtr](const std::error_code& ec, const tcp::endpoint&){
                timerPtr->cancel();
                if(!ec){
                    self->dialogue_->onConnect(*self);
                }else{
                    self->dialogue_->onConnectError(*self, ec.message());
                }
            });
        }else{
            self->dialogue_->onConnectError(*self, ec.message());
        }
    });
}

}

#ifdef TEVHDRONLY
#include "src/tcpRequest.cxx"
#endif

#endif