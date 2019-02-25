cmake_minimum_required(VERSION 2.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

file(GLOB SRC
    "source/*.cpp"
)

add_executable(obd_reader ${SRC})
target_link_libraries( obd_reader ${Boost_LIBRARIES} )