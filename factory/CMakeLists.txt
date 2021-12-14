#factory

include_directories(base/)
include_directories(main/)
include_directories(sub/APIs/)
include_directories(sub/factories/)

link_directories( ../build/CLi )
link_directories( ../build/core )
link_directories( ../bild/containers )

add_library( factory base/acceptance.cpp )

