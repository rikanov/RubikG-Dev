#include <cli.h>

#ifdef _WIN32
void CLi::REPL(std::istream & IS, std::ostream & OS)
{
  ;
}
#else

#include <readline/readline.h>
#include <readline/history.h>

void CLi::REPL(std::istream & IS, std::ostream & OS)
{
  if(&IS==&std::cin)
  {
    NL( Color::off );
    clog( Color::red, "To log out from REPL press Ctrl-D or Ctrl-Z on Windows systems" );
    
    // Use tab for auto completion
    rl_bind_key ('\t', rl_complete) ;
    
    // Use our function for auto - complete
    rl_attempted_completion_function = completion ;
    
    // set prompt
    const char prompt[] = "REPL > ";
    while( true )
    {
      clog( Color::white );
      char* buf=readline( prompt );
      if ( buf == NULL )
      {
        NL( Color::off );
        clog( Color::red, "\nREPL mode has been closed.", Color::off);
        free(buf);
        return;
      }
      if( strcmp( buf, "" ) != 0 )
      {
        add_history(buf);
      } 
    }
  }
  else
  { 
    ; // ToDo
  }
}
#endif
