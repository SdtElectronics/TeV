// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef tevtcpbase
#define tevtcpbase

#include "internal/baseSession.h"

namespace tev{

using asio::ip::tcp;

class TCPsession: public internal::BaseSession<TCPsession, tcp::socket>{
  public:
    TEVINLINE 
    TCPsession(tcp::socket socket, std::shared_ptr<Session> dialogue, std::size_t maxLength);

    // Close session at timePoint
    template <typename CB>
    Timer expireAt(const Timer::TimePoint& timePoint, CB&& callback);
    // Close session after timeout
    template <typename Rep, typename Period>
    Timer expire(const Timer::Duration<Rep, Period>& timeout);
    // Invoke callback and close session after timeout
    template <typename Rep, typename Period, typename CB>
    Timer expire(const Timer::Duration<Rep, Period>& timeout, CB&& callback);

    TEVINLINE void close();

    TEVINLINE std::string getRemoteIP() const;
    TEVINLINE unsigned short getRemotePort() const;

};

template<typename Rep, typename Period>
Timer TCPsession::expire(const Timer::Duration<Rep, Period>& timeout){
    Timer timer(timeout);
    timer.start([this](const std::error_code& ec){
        close();
    });
    return timer;
}

template<typename CB>
Timer TCPsession::expireAt(const Timer::TimePoint& timePoint, CB&& callback){
    Timer timer(timePoint);
    timer.start([this, TEVCAPTMOVE(callback](const std::error_code& ec){
        callback(ec);
        close();
    }));
    return timer;
}

template<typename Rep, typename Period, typename CB>
Timer TCPsession::expire(const Timer::Duration<Rep, Period>& timeout, CB&& callback){
    Timer timer(timeout);
    timer.start([this, TEVCAPTMOVE(callback](const std::error_code& ec){
        callback(ec);
        close();
    }));
    return timer;
}

}

#ifdef TEVHDRONLY
#include "src/tcpSession.cxx"
#endif

#endif