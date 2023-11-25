cmake_minimum_required(VERSION 3.12)
ListDirectories(packages ${PROJECT_SOURCE_DIR}/packages)

foreach(package ${packages})
  message(NOTICE "find package ${ROOT_PROJECT_NAME}.${package}")
  set(PACKAGE_NAME ${ROOT_PROJECT_NAME}.${package})
  add_subdirectory(${package})
endforeach()
