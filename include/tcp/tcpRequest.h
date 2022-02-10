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
    TCPrequest(dialoguePtr dialogue, asio::io_context& asio_ctx, std::size_t maxLength = 65535);
    // send request to hostname:port
    TEVINLINE void send(const char* hostname, const char* port);

  private:
    TEVINLINE void asyncConnect(const tcp::resolver::results_type& results);

    tcp::resolver resolver_;
    asio::io_context& asio_ctx_;
};

}

#ifdef TEVHDRONLY
#include "src/tcpRequest.cxx"
#endif

#endif