// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef tevsession_
#define tevsession_

#include "session.h"

namespace tev{
namespace internal{

class Session_: public Session{
  public:
    virtual        void onConnect   (TCPsession& session) override{}
    virtual        void onDisconnect(TCPsession& session) override{}
    virtual inline      ~Session_()                       override{}
};

template <typename CB, typename T>
class Session_onConnect: public T{
  private:
    CB callback_;
  
  public:
    Session_onConnect(const CB& cb, const T& t): T(t), callback_(cb){}
    virtual void onConnect(TCPsession& session) override{
        callback_(session);
    }
};

template <typename CB, typename T>
class Session_onDisconnect: public T{
  private:
    CB callback_;
  
  public:
    Session_onDisconnect(const CB& cb, const T& t): T(t), callback_(cb){}
    virtual void onDisconnect(TCPsession& session) override{
        callback_(session);
    }
};

template <typename CB, typename T>
class Session_onConnectError: public T{
  private:
    CB callback_;
  
  public:
    Session_onConnectError(const CB& cb, const T& t): T(t), callback_(cb){}
    virtual void onConnectError(TCPsession& session, const std::string& err) override{
        callback_(session, err);
    }
};

template <typename CB, typename T>
class Session_onReadError: public T{
  private:
    CB callback_;
  
  public:
    Session_onReadError(const CB& cb, const T& t): T(t), callback_(cb){}
    virtual void onReadError(TCPsession& session, const std::string& err) override{
        callback_(session, err);
    }
};

template <typename CB, typename T>
class Session_onWriteError: public T{
  private:
    CB callback_;
  
  public:
    Session_onWriteError(const CB& cb, const T& t): T(t), callback_(cb){}
    virtual void onWriteError(TCPsession& session, const std::string& err) override{
        callback_(session, err);
    }
};

template <typename CB, typename T>
class Session_onError: public T{
  private:
    CB callback_;
  
  public:
    Session_onError(const CB& cb, const T& t): T(t), callback_(cb){}
    virtual void onError(TCPsession& session, const std::string& err, ErrorType type) override{
        callback_(session, err, type);
    }
};

}
}
#endif