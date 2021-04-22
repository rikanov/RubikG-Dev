#ifndef COLORS_DEFINED__
#define COLORS_DEFINED__

#include <iostream>


#ifdef _WIN32
  #include <windows.h>   // WinApi header
  static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

namespace Color 
{
  enum Code {
#ifdef _WIN32
    DEFAULT       = 7, 
	BOLD          = FOREGROUND_INTENSITY,
	ULINE         = FOREGROUND_INTENSITY,
	FLASH         = FOREGROUND_INTENSITY,
    BLACK         = 0,
    BLUE          = 1,
    GREEN         = 2,
    CYAN          = 3,
    RED           = 4,
    MAGENTA       = 5,
    BROWN         = 6,
    LIGHT_GRAY    = 7, 
    DARK_GRAY     = 8, 
    LIGHT_BLUE    = 9, 
    LIGHT_GREEN   = 10, 
    LIGHT_CYAN    = 11,
    LIGHT_RED     = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_YELLOW  = 14,
    WHITE         = 15
#else
    DEFAULT       = 0,
    BOLD          = 1,
    ULINE         = 4,
    FLASH         = 5,
    BLACK         = 30, 
    RED           = 31, 
    GREEN         = 32, 
    YELLOW        = 33, 
    BLUE          = 34, 
    MAGENTA       = 35, 
    CYAN          = 36, 
    LIGHT_GRAY    = 37, 
    DARK_GRAY     = 90, 
    LIGHT_RED     = 91, 
    LIGHT_GREEN   = 92, 
    LIGHT_YELLOW  = 93, 
    LIGHT_BLUE    = 94, 
    LIGHT_MAGENTA = 95, 
    LIGHT_CYAN    = 96, 
    WHITE         = 97
#endif
  };

  class Modifier 
  {
    Code m_code;
  public:
    Modifier() : m_code(DEFAULT) {}
    Modifier(Code pCode) : m_code(pCode) {}
    
    Code code() const 
    {
      return m_code;
    }
    
    friend std::ostream& operator<<( std::ostream& os, const Modifier& mod ) 
    {
#ifdef _WIN32
      SetConsoleTextAttribute(hConsole, mod.code() );
      return os;
#else
      return os << "\033[" << mod.code() << "m";
#endif
    }
    
    friend std::wostream& operator<<( std::wostream& os, const Modifier& mod ) 
    {
#ifdef _WIN32
      SetConsoleTextAttribute(hConsole, mod.code() );
      return os;
#else
      return os << "\033[" << mod.code() << "m";
#endif
    }
    
  };
  
  extern Modifier off;
  extern Modifier bold;
  extern Modifier uline;
  extern Modifier flash;
  extern Modifier black;
  extern Modifier red;
  extern Modifier blue;
  extern Modifier green;
  extern Modifier cyan;
  extern Modifier gray;
  extern Modifier dark;
  extern Modifier light;
  extern Modifier yellow;
  extern Modifier white;
}

#endif // COLORS_DEFINED__
