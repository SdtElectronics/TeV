#include <string>

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "sharedBuf.h"
#include "testBuf.h"
#include "worker.h"

TEST_CASE("SharedBuffer write and retrieval"){
    tev::SharedBuffer sd(std::string("first"));

    SECTION("Write and retrieve single chunk"){
        TestStream ts(tev::Worker::getContext());
        ts.write_some(sd);
        char* buf = ts.getBuf();
        REQUIRE(strncmp(buf, "first", 5) == 0);
    }

    SECTION( "resizing bigger changes size and capacity" ){
        TestStream ts(tev::Worker::getContext());
        sd.append(std::string("second"));
        ts.write_some(sd);
        char* buf = ts.getBuf();
        REQUIRE(strncmp(buf, "firstsecond", 11) == 0);
    }
}