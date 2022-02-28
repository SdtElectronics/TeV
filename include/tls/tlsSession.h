// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef tevtlsbase
#define tevtlsbase

#include "internal/baseSession.h"

namespace tev{

using asio::ssl;

class TLSsession: public internal::BaseSession<TLSsession>{
  public:
    TEVINLINE
    TLSsession(
        ssl::context ctx, 
        tcp::socket socket, 
        std::shared_ptr<Session> dialogue, 
        std::size_t maxLength);

  private:

};

}