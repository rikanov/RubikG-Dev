#engine


include_directories(include)
include_directories( ../CLi/include)
include_directories(template_h)

link_directories( ../build/CLi )

add_library( engine     source/acceptance.cpp )
