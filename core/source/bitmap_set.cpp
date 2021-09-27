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

  // skip zero valued bits
  for( ; ( m_dataSet & 1 ) == 0; m_dataSet >>= 1, ++ m_nextID );

  m_dataSet >>= 1;
  nextID = m_nextID ++;
  return true;
}

uint8_t BitMap::next()
{
  if( m_dataSet == 0 )
  {
    return 0xFF;
  }

  // skip zero valued bits
  for( ; ( m_dataSet & 1 ) == 0; m_dataSet >>= 1, ++ m_nextID );

  return m_nextID;
}

void BitMap::Print( const BitMapID dataSet, const uint8_t length, const uint8_t slice )
{
  const Color::Modifier color[2] = { Color::blue, Color::green };
  for( long id = length - 1; id >= 0; --id )
  {
    if ( slice > 0 && ( length - 1 - id ) % slice == 0 )
    {
      slog_ ( "", Color::white, '|', Color::off );
    }
    uint64_t mask = ( 1ULL << id );
    const bool bit = ( dataSet & mask ) == mask;
    slog_ ( "", color[bit], bit, Color::off );
  }
  NL();
}

void BitMap::Print( const BitMap32ID dataSet, const uint8_t length, const uint8_t slice )
{
  Print( (BitMapID) dataSet, length, slice );
}

void BitMap::print(const uint8_t length, const uint8_t slice) const
{
  Print( m_dataSet, length, slice );
}
