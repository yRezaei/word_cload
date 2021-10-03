cmake_minimum_required(VERSION 3.16)
project(word_cloud)

set(CMAKE_CXX_STANDARD 17)
set(WARN -Wall -Wextra -Werror -Wfatal-errors)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../out)

add_executable(word_cloud app.cxx)
target_compile_options(word_cloud PUBLIC ${WARN})
