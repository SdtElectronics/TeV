// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef tevtcpbase
#define tevtcpbase

#include <memory>
#include <utility>

#include "internal/baseSession.h"

namespace tev{

using asio::ip::tcp;

class TCPsession: public internal::BaseSession<TCPsession, tcp::socket>{
  public:
    TEVINLINE 
    TCPsession(tcp::socket socket, std::shared_ptr<Session> dialogue, std::size_t maxLength);

    // Close session at timePoint
    template<typename CB>
    Timer expireAt(const TimePoint& timePoint, CB&& callback);
    // Close session after timeout
    template<typename Rep, typename Period>
    Timer expire(const duration<Rep, Period>& timeout);
    // Invoke callback and close session after timeout
    template<typename Rep, typename Period, typename CB>
    Timer expire(const duration<Rep, Period>& timeout, CB&& callback);

    TEVINLINE void close();

    TEVINLINE std::string getRemoteIP() const;
    TEVINLINE unsigned short getRemotePort() const;

};

template<typename Rep, typename Period>
TCPsession::Timer TCPsession::expire(const TCPsession::duration<Rep, Period>& timeout){
    return defer(timeout, [this](const std::error_code& ec){
        close();
    });
}

template<typename CB>
TCPsession::Timer TCPsession::expireAt(const TCPsession::TimePoint& timePoint, CB&& callback){
    return deferAt(timePoint, [this](const std::error_code& ec){
        close();
    });
}

template<typename Rep, typename Period, typename CB>
TCPsession::Timer TCPsession::expire(
    const TCPsession::duration<Rep, Period>& timeout, CB&& callback){
    return std::move(defer(timeout, [this, TEVCAPTMOVE(callback)](const std::error_code& ec){
        callback(ec);
        close();
    }));
}

}

#ifdef TEVHDRONLY
#include "src/tcpSession.cxx"
#endif

#endif