#ifndef CLI_HEADER
#define CLI_HEADER

#include <vector>
#include <iostream>
#include <text_output.h>

constexpr int  CURRENT_VERSION_NUMBERS[] = { 2, 0, 0 };

class CLi
{
  static int                         m_predictionCount;
  static std::vector<const char*> *  m_commands;
  
  static void setBaseCommands ();

  static int     initCLi     ( void );
  static void    intro       ( void );
  static char ** completion  ( const char * text, int start, int end ); 
  static char *  getCommands ( const char * text, int state          );
  
public:
  static void init ( void );
  static void exit ( void );
  static void REPL ( std::istream & IS = std::cin, std::ostream & OS = std::cout );
  
};

#endif
