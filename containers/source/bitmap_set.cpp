#include <bitmap_set.h>
#include <text_output.h>

BitMap::BitMap( const BitMap::Init & iList )
  : m_dataSet( 0 )
  , m_nextID( 0 )
{
  for ( auto P : iList )
  {
    add( P );
  }
}

uint8_t BitMap::next()
{
  BitMapID step = 1ULL << m_nextID;
  // skip zero valued bits
  for( ; ( m_dataSet & step ) == 0; step <<= 1 )
  {
     ++ m_nextID;
  }

  m_dataSet -= step;

  return m_nextID ++ ;
}

bool BitMap::next( uint8_t & nextID )
{
  if( 0 == m_dataSet )
  {
    return false;
  }

  BitMapID step = 1ULL << m_nextID;
  // skip zero valued bits
  for( ; ( m_dataSet & step ) == 0; step <<= 1 )
  {
     ++ m_nextID;
  }

  m_dataSet -= step;
  nextID = m_nextID ++;
  return true;
}

void BitMap::Print_( const BitMapID dataSet, const uint8_t length, const uint8_t slice )
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
}


void BitMap::Print( const BitMapID dataSet, const uint8_t length, const uint8_t slice )
{
  Print_( (BitMapID) dataSet, length, slice );
  NL();
}

void BitMap::Print_( const BitMap32ID dataSet, const uint8_t length, const uint8_t slice )
{
  Print_( (BitMapID) dataSet, length, slice );
}

void BitMap::Print( const BitMap32ID dataSet, const uint8_t length, const uint8_t slice )
{
  Print( (BitMapID) dataSet, length, slice );
}

void BitMap::print_(const uint8_t length, const uint8_t slice) const
{
  Print_( m_dataSet, length, slice );
}

void BitMap::print(const uint8_t length, const uint8_t slice) const
{
  Print( m_dataSet, length, slice );
}
