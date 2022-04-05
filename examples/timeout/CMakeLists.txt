add_executable(timeoutServer
    timeoutServer.cxx
)
target_link_libraries(timeoutServer 
  PRIVATE
    tev
)

add_executable(timeoutClient
    timeoutClient.cxx
)
target_link_libraries(timeoutClient 
  PRIVATE
    tev
)