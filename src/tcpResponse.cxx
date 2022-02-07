// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef TEVHDRONLY
#include "internal/tcpResponse.h"
#endif

namespace tev{
namespace internal{

TCPresponse::TCPresponse(tcp::socket socket, 
                         std::shared_ptr<Session> dialogue, 
                         std::size_t maxLength):
    TCPsession(std::move(socket), dialogue, maxLength){
}

void TCPresponse::start(){
    dialogue_->onConnect(*this);
}

} // namespace internal
} // namespace tev