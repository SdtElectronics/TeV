// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef tevtcpsession
#define tevtcpsession

#include <memory>
#include <utility>

#include "asio.hpp"
#include "internal/tevdefs.h"
#include "session.h"
#include "tcp/tcpSession.h"

namespace tev{
namespace internal{

using asio::ip::tcp;

class TCPresponse: public TCPsession{
  public:
    TEVINLINE
    TCPresponse(tcp::socket socket, std::shared_ptr<Session> session, std::size_t maxLength);

    TEVINLINE void start();
};

} // namespace internal
} // namespace tev

#ifdef TEVHDRONLY
#include "src/tcpResponse.cxx"
#endif

#endif