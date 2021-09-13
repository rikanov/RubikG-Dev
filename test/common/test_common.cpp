#include <test.h>
#include <text_output.h>

void UnitTests::head( const std::string& unit )
{
  CLS();
  clog( Color::light, Color::flash, ">>>>>  ", Color::off, Color::gray, "Starting unit test of", Color::white,  unit);
  clog(               "       ", std::string( 22 + unit.length(), '-' ) );
  NL();
}

void UnitTests::tail( const std::string& unit, bool success )
{
  NL();
  clog_( Color::white, unit, "test result:" );
  if ( success )
  {
    clog( Color::green, "SUCCEED", Color::off );
  }
  else
  {
    clog( Color::red, Color::flash, "FAILED!", Color::off );
  }
  NL();
}

void UnitTests::stamp( bool success, bool& result )
{
  if ( success )
  {
    clog( Color::green, "OK", Color::off );
  }
  else
  {
    clog( Color::red, Color::bold, Color::flash, "NO OK!", Color::off );
  }
  result &= success;
}

void UnitTests::finish( const std::string& unit, bool success )
{
  NL( Color::off );
  clog_( Color::gray, "Unit test of", Color::white,  unit, Color::off, ':');
  if ( success )
  {
    clog_( Color::green, Color::bold, "SUCCEED", Color::off );
  }
  else
  {
    clog_( Color::red, Color::bold, Color::flash, "FAILED!", Color::off );
  }
  clog( Color::light, Color::flash, "  <<<<<", Color::off );
  clog( std::string( 23 + unit.length(), '-' ) );
  NL();
  NL();  
  clog( Color::off, "Press", Color::white, Color::bold, "ENTER", Color::off,"to end." ); 
  std::cin.get();
}

void UnitTests::tcase( const std::string& unit, const std::string& msg1, const std::string& msg2 )
{
  slog_( "", Color::white, Color::uline, unit, Color::off, ": ");
  if( ! msg1.empty() )
  {
    clog_( Color::gray, msg1 );
  }
  if( ! msg2.empty() )
  {
    clog_( "-->", msg2 );
  }
  NL( Color::off );
}


void UnitTests::tcase( const std::string& unit, const std::string& msg, const long num )
{
  slog( "", Color::white, Color::uline, unit, Color::gray, msg, num, ':', Color::off );
}

void UnitTests::done()
{ 
  clog( Color::white, Color::bold, "Done.", Color::off ); 
} 

