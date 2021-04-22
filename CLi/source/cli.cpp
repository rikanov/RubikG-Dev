#include <text_output.h>
#include <cli.h>


#include <iostream>
#include <locale>

std::vector<const char*> * CLi::m_commands = new std::vector<const char*>;
int CLi::m_predictionCount = 0;

int CLi::initCLi( void )
{
  clog( "User-preferred locale setting is" , std::locale("").name().c_str() );
  
  // replace the C++ global locale as well as the C locale with the user-preferred locale
  std::locale::global( std::locale( "" ) );
  
  // use the new global locale for future special character output
  std::cout.imbue( std::locale( "" ) );
  
  return 0; // ToDo: error handling
}

void CLi::init()
{
  initCLi();
  intro();
  m_predictionCount = 0;
  setBaseCommands();
}

void CLi::exit()
{
  delete m_commands;
  m_commands = nullptr;
}

void CLi::setBaseCommands()
{
  m_commands->push_back( "#unit_tests" );
  m_commands->push_back( "editor" ); 
}
