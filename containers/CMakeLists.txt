#containers

include_directories(include/)

link_directories( ../build/CLi )
link_directories( ../build/core )

add_library( containers
                    source/sequence.cpp
                    source/cube_set.cpp
                    source/bitmap_set.cpp
)
