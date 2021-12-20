#CLi
include_directories(include)


add_library(cli source/_intro_screen.cpp
                source/def_colors.cpp 
                source/cli.cpp
                source/repl.cpp
                source/repl_autocomp.cpp
                source/text_output.cpp
            )
