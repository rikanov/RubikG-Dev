cmake_minimum_required(VERSION 3.0)

project(rubikg)

include_directories( rubikAI/include )
include_directories( engine/include )
include_directories( engine/interface )
include_directories( engine/factories )
include_directories( engine/APIs )
include_directories( engine/services )
include_directories( progress/ )
include_directories( factory/sub )
include_directories( factory/main )
include_directories( core/include )
include_directories( core/template_h )
include_directories( CLi/include )
include_directories( test )

add_subdirectory(engine)
add_subdirectory(core)
add_subdirectory(CLi)
add_subdirectory(test)

add_executable(rubikg main.cpp)

set_property(TARGET rubikg PROPERTY CXX_STANDARD 17)
target_link_libraries(rubikg core cli test)
