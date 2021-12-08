cmake_minimum_required(VERSION 3.0)

project(rubikg)

include_directories( progress/ )
include_directories( factory/base/ )
include_directories( factory/sub/ )
include_directories( factory/main )
include_directories( containers/include/ )
include_directories( core/include )
include_directories( core/template_h )
include_directories( CLi/include )
include_directories( test )

add_subdirectory(factory)
add_subdirectory(containers)
add_subdirectory(core)
add_subdirectory(CLi)
add_subdirectory(test)

add_executable(rubikg main.cpp)

set_property(TARGET rubikg PROPERTY CXX_STANDARD 17)
target_link_libraries(rubikg factory containers core cli test)
