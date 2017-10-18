link_directories(/usr/local/lib)

add_executable(central_server main.c)
add_executable(exampleclient exampleclient.c)
target_link_libraries(central_server gnutls)
target_link_libraries(exampleclient gnutls)


add_subdirectory(lib)
