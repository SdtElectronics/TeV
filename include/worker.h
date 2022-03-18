// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef tevworker
#define tevworker

#include "asio/asio.hpp"

namespace tev{

class Worker{
  public:
    static inline asio::io_context& getContext(){
        static asio::io_context ctx;
        return ctx;
    }

    static inline void run(){
        getContext().run();
    }
};

}

#endif