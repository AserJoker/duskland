cmake_minimum_required(VERSION 3.12)
project(duskland.exec)

aux_source_directory(./src ${PROJECT_NAME}.src)
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}.src})

foreach(package ${packages})
    message(STATUS "link package ${package}")
    target_link_libraries(${PROJECT_NAME} ${ROOT_PROJECT_NAME}.${package})
endforeach()

target_link_libraries(${PROJECT_NAME} ncursesw)
target_link_libraries(${PROJECT_NAME} cjson)
target_link_libraries(${PROJECT_NAME} lua5.3)