// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef TEVHDRONLY

#include "tls/tlsSession.h"
#endif

namespace tev{

TCPsession::TCPsession (tcp::socket socket, 
                        std::shared_ptr<Session> dialogue, 
                        std::size_t maxLength): 
            BaseSession<TCPsession, tcp::socket>(std::move(socket), dialogue, maxLength){
}

void TCPsession::close(){
    std::error_code ec;
    stream_.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
    stream_.close();
}

std::string TCPsession::getRemoteIP() const{
    return stream_.remote_endpoint().address().to_string();
}

unsigned short TCPsession::getRemotePort() const{
    return stream_.remote_endpoint().port();
}

}