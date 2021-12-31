#test

include_directories($(CMAKE_PROJECT_INCLUDE))

#find_library( cli  REQUIRED )
#find_library( core REQUIRED )

add_library(test  test.h

                  common/test_common.cpp
                  common/test_random.cpp
                  common/test_launchers.cpp
                  common/test_timer.cpp

                  core/u_simplex.cpp
                  core/u_cubeset.cpp
                  core/u_crotations.cpp
                  core/u_genrotationset.cpp
                  core/u_rubik.cpp

                  progress/u_progress.cpp
                 )


target_link_libraries( test cli core containers )
