#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "testBuf.h"
#include "timer.h"

TEST_CASE("Timer"){
    SECTION("Invoke callback after timeout"){
        auto future = std::chrono::steady_clock::now() + std::chrono::seconds(1);
        tev::Timer timer(std::chrono::seconds(1));
        timer.start([&future](const std::error_code& ec){
            REQUIRE(future > std::chrono::steady_clock::now());
        });
    }

    SECTION("Invoke callback at timepoint"){
        auto future = std::chrono::steady_clock::now() + std::chrono::seconds(1);
        tev::Timer timer(future);
        timer.start([&future](const std::error_code& ec){
            REQUIRE(future > std::chrono::steady_clock::now());
        });
    }
    
    tev::Worker::run();
}