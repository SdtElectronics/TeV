add_executable(httpClient
    httpClient.cxx
)
target_link_libraries(httpClient 
  PRIVATE
    tev
)