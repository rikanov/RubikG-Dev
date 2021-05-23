#ifndef TEXT_OUTPUT__H
#define TEXT_OUTPUT__H


#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>

#include <def_colors.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
 constexpr char FChar[] = { char(254), char(0) };
#else
 constexpr char FChar[] = "▄";
#endif

static const Color::Modifier Default = Color::off;

// clear screen ToDo
inline void CLS()
{
  if ( system( "CLS" ) ) system( "clear" );
}

// new line
inline void NL()
{
  std::cout << std::endl;
}

// set text color & new line
inline void NL( Color::Modifier c )
{
  std::cout << c << std::endl;
}

  // basic text messages
 // --------------------
template <typename T>
void clog_ (T t) 
{
  if ( typeid( t ) != typeid( Color::Modifier ) )
  {
    std::cout << t << ' ';
  }
  else
  {
    std::cout << t;
  }
}

template <typename T, typename... Args>
void clog_ (T t, Args... args) 
{
  clog_ ( t );
  clog_ ( args... );
  std::cout << std::flush;
}

template <typename T>
void clog (T t) 
{
  std::cout << t << std::endl ;
}

template<typename T, typename... Args>
void clog (T t, Args... args) // recursive variadic function
{
  clog_ ( t );
  clog  ( args... ) ;
}

 // basic text messages with given separator
// -----------------------------------------
template < typename T>
void slog_ ( const char * sep, T t) 
{
  if ( typeid( t ) != typeid( Color::Modifier ) )
  {
    std::cout << t << sep;
  }
  else
  {
    std::cout << t;
  }
}
template < typename T, typename... Args>
void slog_ ( const char * sep, T t, Args... args ) 
{
  slog_ ( sep, t );
  slog_ ( sep, args... );
}

template < typename T >
void slog ( const char * sep, T t) 
{
  std::cout << t << std::endl ;
}

template< typename T, typename... Args >
void slog ( const char * sep, T t, Args... args) // recursive variadic function
{
  slog_ ( sep, t );
  slog  ( sep, args... ) ;
}

// logger
inline void clogPos(const char * text)
{
  clog( text, std::string("\t@  ") + std::string(__FILE__) + std::to_string(__LINE__) );
}

#endif // ! TEXT_OUTPUT__H
