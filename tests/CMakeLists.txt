add_executable(sharedBuf
    sharedBuf.cxx
)
add_test(SharedBuf sharedBuf)
target_link_libraries(sharedBuf
  PRIVATE
    tev
    asio
    Catch2
)

add_executable(timer
    timer.cxx
)
add_test(Timer timer)
target_link_libraries(timer
  PRIVATE
    tev
    asio
    Catch2
)

if(MSVC)
    set_target_properties(
        sharedBuf
        timer

      PROPERTIES 
        COMPILE_OPTIONS /W4
    )
else()
    set_target_properties(
        sharedBuf
        timer

      PROPERTIES 
        COMPILE_OPTIONS -Wall -Wextra -Wpedantic
    )
endif()