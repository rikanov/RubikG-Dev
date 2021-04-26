#test

include_directories($(CMAKE_PROJECT_INCLUDE))
include_directories(../core/include)
include_directories(../core/template_h)
include_directories(../CLi/include)
include_directories(../engine )

#find_library( cli  REQUIRED )
#find_library( core REQUIRED )

add_library(test  test.h

                  common/test_common.cpp
                  common/test_launchers.cpp
                  common/test_timer.cpp

                  core/u_simplex.cpp 
                  core/u_crotations.cpp
                  core/u_cextrotations.cpp
                  core/u_rubik.cpp

                  engine/u_insight.cpp
                  engine/u_engine.cpp
                 )


target_link_libraries( test cli core )
