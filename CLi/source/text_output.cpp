#include <text_output.h>

bool LoggingON = true;


std::string numL( const uint64_t num, const size_t size )
{
  std::string out = std::to_string( num );
  if ( size > out.size() )
    out += std::string( size - out.size(), ' ' );
  return out;
}

std::string numR( const uint64_t num, const size_t size )
{
  std::string out = std::to_string( num );
  if ( size > out.size() )
    out = std::string( size - out.size(), ' ' ) + out;
  return out;
}

// draw line
void cdraw( const char c, int p )
{
  clog( std::string( p, c ) );
}
