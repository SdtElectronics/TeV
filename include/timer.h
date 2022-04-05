// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef tevtimer
#define tevtimer

#include "asio.hpp"
#include "internal/tevdefs.h"
#include "worker.h"

namespace tev{

class Timer: public asio::steady_timer{
  public:
    template <typename Rep, typename Period>
    using Duration = std::chrono::duration<Rep, Period>;
    using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

    // Construct a timer fires at timePoint
    inline Timer(const TimePoint& timePoint);

    // Construct a timer fires after timeout
    template <typename Rep, typename Period>
    Timer(const Duration<Rep, Period>& timeout);

    template <typename CB>
    void start(CB&& callback);
};

Timer::Timer(const Timer::TimePoint& timePoint): 
    asio::steady_timer(Worker::getContext(), timePoint){
}

template <typename Rep, typename Period>
Timer::Timer(const Duration<Rep, Period>& timeout): 
    Timer(std::chrono::steady_clock::now() + timeout){
}

template <typename CB>
void Timer::start(CB&& callback){
    async_wait([TEVCAPTMOVE(callback)](const std::error_code& ec){
        if(ec != asio::error::operation_aborted){
            callback(ec);
        }
    });
}

}

#endif