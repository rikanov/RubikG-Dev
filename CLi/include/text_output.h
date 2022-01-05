#ifndef TEXT_OUTPUT__H
#define TEXT_OUTPUT__H

#include <iostream>
#include <cstring>
#include <string>

#include <def_colors.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <conio.h>
 constexpr char FChar[] = { char(254), char(0) };
#else
 constexpr char FChar[] = "▄";
#endif

static const Color::Modifier Default = Color::off;

// clear screen ToDo
inline void CLS()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  clrscr(); // including header file : conio.h
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
  std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences 
#elif defined (__APPLE__)
  system("clear");
#endif
}


#define OUT if(!LoggingON)return;

extern bool LoggingON;

// new line
inline void NL()
{
  OUT
  std::cout << std::endl;
}

// set text color & new line
inline void NL( Color::Modifier c )
{
  OUT
  std::cout << c << std::endl;
}

  // basic text messages
 // --------------------
template <typename T>
void clog_ (T t) 
{
  OUT
  if ( typeid( t ) != typeid( Color::Modifier ) )
  {
    std::cout << t << ' ';
  }
  else
  {
    std::cout << t;
  }
  std::cout << std::flush;
}

template <typename T, typename... Args>
void clog_ (T t, Args... args) 
{
  OUT
  clog_ ( t );
  clog_ ( args... );
}

template <typename T>
void clog (T t) 
{
  OUT
  std::cout << t << std::endl ;
}

template<typename T, typename... Args>
void clog (T t, Args... args) // recursive variadic function
{
  OUT
  clog_ ( t );
  clog  ( args... ) ;
}

 // basic text messages with given separator
// -----------------------------------------
template< typename T>
void slog_ ( const char * sep, T t) 
{
  OUT
  if ( typeid( t ) != typeid( Color::Modifier ) )
  {
    std::cout << t << sep;
  }
  else
  {
    std::cout << t;
  }
}
template< typename T, typename... Args>
void slog_ ( const char * sep, T t, Args... args ) 
{
  OUT
  slog_ ( sep, t );
  slog_ ( sep, args... );
}

template< typename T >
void slog ( const char * sep, T t) 
{
  OUT
  std::cout << t << sep << std::endl ;
}

template< typename T, typename... Args >
void slog( const char * sep, T t, Args... args) // recursive variadic function
{
  OUT
  slog_ ( sep, t );
  slog  ( sep, args... ) ;
}

std::string numL( const uint64_t num, const size_t size );
std::string numR( const uint64_t num, const size_t size );

// draw line
void cdraw( const char c, int p );

#undef OUT
#endif // ! TEXT_OUTPUT__H
