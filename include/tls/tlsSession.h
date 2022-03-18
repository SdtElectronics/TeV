// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef tevtlsbase
#define tevtlsbase

#include "internal/baseSession.h"

namespace tev{

using asio::ssl;

class TLSsession: public internal::BaseSession<TCPsession, ssl::stream<tcp::socket> >{
  public:
    TEVINLINE
    TLSsession(
        tcp::socket socket, 
        std::shared_ptr<Session> dialogue, 
        std::size_t maxLength);

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

}