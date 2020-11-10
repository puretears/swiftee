cmake_minimum_required(VERSION 3.15)

project(Tidus VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(VM)
add_subdirectory(Parser)
add_subdirectory(Common)

list(APPEND EXTRA_LIBS VM Parser Unicode)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/VM" "${PROJECT_SOURCE_DIR}/Parser" "${PROJECT_SOURCE_DIR}/Common")

add_executable(tidus main.cpp)

target_link_libraries(tidus PUBLIC ${EXTRA_LIBS})
target_include_directories(tidus PUBLIC ${EXTRA_INCLUDES})
