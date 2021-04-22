#include <cli.h>
#include <stdio.h>
#include <vector>

#ifdef _WIN32
char ** CLi::completion( const char * stem_text,int start,int end ) 
{
  char ** matches = NULL;
  return matches ;
}

char * CLi::getCommands( const char * stem_text,int state ) 
{

 // No more matches
  return nullptr;
}
#else

#include <readline/readline.h>

char ** CLi::completion( const char * stem_text,int start,int end ) 
{
  char ** matches = NULL;
  matches = rl_completion_matches( stem_text, getCommands );
  return matches ;
}


char * CLi::getCommands( const char * stem_text,int state ) 
{
 
 if( state == 0) 
 {
   // We initialize the m_predictionCount the first time we are called
   // with this stem_text
   m_predictionCount = -1;
 }

 int text_len = strlen ( stem_text ) ;

 // Search through the command vector until we find a match
 while ( m_predictionCount < (int) m_commands->size() - 1 ) 
 {
   ++m_predictionCount;
   if( strncmp ( m_commands->at( m_predictionCount ) , stem_text , text_len ) == 0) 
   {
     // Must return a duplicate , Readline will handle
     // freeing this string itself .
     return strdup ( m_commands->at( m_predictionCount ) ) ;
   }
 }

 // No more matches
  return nullptr;
}
#endif
