link_directories(/usr/local/lib)

set(LIBS ${LIBS} mariadb gnutls pthread AVL stringMap configLoader linkedList)

add_executable(central_server clientHandler.c main.c)
add_executable(exampleclient exampleclient.c)
target_link_libraries(central_server ${LIBS})
target_link_libraries(exampleclient ${LIBS})


add_subdirectory(lib)
