#include <bitmap_set.h>
#include <text_output.h>


void BitMap::set( const uint64_t ds )
{
  m_dataSet = ds;
  m_nextID  = 0;
}

bool BitMap::next( uint8_t & nextID )
{
  if( m_dataSet == 0 )
  {
    return false;
  }

  for( ; ( m_dataSet & 1 ) == 0; m_dataSet >>= 1, ++ m_nextID );

  m_dataSet >>= 1;
  nextID = m_nextID ++;
  return true;
}

void BitMap::reverse(const uint8_t a, const uint8_t b)
{

}

void BitMap::print( const uint8_t length ) const
{
  for( long id = 63; id >= 0; --id )
  {
    uint64_t mask = ( 1ULL << id );
    slog_ ( "",  ( m_dataSet & mask ) == mask );
    if ( length > 0 && id % length == 0 )
    {
      slog_ ( "", '.' );
    }
  }
  NL();
}
