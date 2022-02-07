// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef tevtcpservicebuilder
#define tevtcpservicebuilder

#include <memory>

#include "internal/session_.h"

namespace tev{

template<typename T = internal::Session_>
class SessionBuilder{
  private:
    T session_;
    
  public:
    SessionBuilder(T session): session_(session){}
    SessionBuilder(): session_(){}

    class Factory{
      private:
       SessionBuilder<T>& builder_;
      
      public:
        Factory(SessionBuilder<T> builder): builder_(builder){}

        std::shared_ptr<Session> operator() () const{
            auto ret = static_cast<Session*>(new T(builder_.session_));
            return std::shared_ptr<Session>(ret);
        }
    };

    Factory buildFactory(){
        return Factory(*this);
    }

    std::shared_ptr<Session> build(){
        auto ret = static_cast<Session*>(new T(session_));
        return std::shared_ptr<Session>(ret);
    }

    template<typename CB>
    SessionBuilder<internal::Session_onConnect<CB, T> > onConnect(CB&& callback){
        return SessionBuilder<internal::Session_onConnect<CB, T> >(
            internal::Session_onConnect<CB, T>(callback, session_)
        );
    }

    template<typename CB>
    SessionBuilder<internal::Session_onDisconnect<CB, T> > onDisconnect(CB&& callback){
        return SessionBuilder<internal::Session_onDisconnect<CB, T> >(
            internal::Session_onDisconnect<CB, T>(callback, session_)
        );
    }

    template<typename CB>
    SessionBuilder<internal::Session_onConnectError<CB, T> > onConnectError(CB&& callback){
        return SessionBuilder<internal::Session_onConnectError<CB, T> >(
            internal::Session_onConnectError<CB, T>(callback, session_)
        );
    }

    template<typename CB>
    SessionBuilder<internal::Session_onReadError<CB, T> > onReadError(CB&& callback){
        return SessionBuilder<internal::Session_onReadError<CB, T> >(
            internal::Session_onReadError<CB, T>(callback, session_)
        );
    }

    template<typename CB>
    SessionBuilder<internal::Session_onWriteError<CB, T> > onWriteError(CB&& callback){
        return SessionBuilder<internal::Session_onWriteError<CB, T> >(
            internal::Session_onWriteError<CB, T>(callback, session_)
        );
    }

    template<typename CB>
    SessionBuilder<internal::Session_onError<CB, T> > onError(CB&& callback){
        return SessionBuilder<internal::Session_onError<CB, T> >(
            internal::Session_onError<CB, T>(callback, session_)
        );
    }
};

}

#endif