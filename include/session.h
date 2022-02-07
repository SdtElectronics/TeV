// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef tevsession
#define tevsession

#include <memory>
#include <string>

namespace tev{

class TCPsession;

enum class ErrorType{connect, read, write};

class Session{
  public:
    virtual        void onConnect     (TCPsession& session                        ) = 0;
    virtual        void onDisconnect  (TCPsession& session                        ) = 0;
    virtual inline void onConnectError(TCPsession& session, const std::string& err);
    virtual inline void onReadError   (TCPsession& session, const std::string& err);
    virtual inline void onWriteError  (TCPsession& session, const std::string& err);
    virtual inline void onError(TCPsession& session, const std::string& err, ErrorType);
    virtual inline      ~Session()                                                    ;
};

void Session::onConnectError(TCPsession& session, const std::string& err){
    onError(session, err, ErrorType::connect);
}

void Session::onReadError(TCPsession& session, const std::string& err){
    onError(session, err, ErrorType::read);
}

void Session::onWriteError(TCPsession& session, const std::string& err){
    onError(session, err, ErrorType::write);
}

void Session::onError(TCPsession&, const std::string&, ErrorType){
}

Session::~Session(){}

}
#endif