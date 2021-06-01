#core

include_directories(include)
include_directories( ../CLi/include)
include_directories(template_h)

link_directories( ../build/CLi )

add_library(core    source/simplex.cpp 
                    source/oriented_cube.cpp
                    source/sequence.cpp
                    source/cube_set.cpp
            )
