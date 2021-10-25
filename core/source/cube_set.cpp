#include <text_output.h>
#include <cube_set.h>
#include <bitmap_set.h>
#include <simplex.h>

CubeSet * CubeSet::Singleton = nullptr;
const BitMap32ID * CubeSet::m_lowMapLeft  = nullptr;
const BitMap32ID * CubeSet::m_highMapLeft = nullptr;
const BitMap32ID * CubeSet::m_lowMapRight = nullptr;
const BitMap32ID * CubeSet::m_highMapRight= nullptr;

BitMap32ID CubeSet::MultipleRotation( const CubeID state, BitMapID cubeSet, const CubeSet::TransMode tr, const bool swp )
{
  BitMap32ID result = 0;
  if( tr == CubeSet::trHigh )
  {
    cubeSet <<= 12;
  }
  BitMap operations( cubeSet );
  if ( swp )
  {
    for ( CubeID next; operations >> next; )
    {
      result |= ( 1 << Simplex::Composition( state, next ) );
    }
  }
  else
  {
    for ( CubeID next; operations >> next; )
    {
      result |= ( 1 << Simplex::Composition( next, state ) );
    }
  }
  return result;
}

CubeSet::CubeSet()
{
  BitMap32ID * lowMapLeft  = new BitMap32ID [ 4096 * 24 ];
  BitMap32ID * highMapLeft = new BitMap32ID [ 4096 * 24 ];
  
  BitMap32ID * lowMapRight  = new BitMap32ID [ 4096 * 24 ];
  BitMap32ID * highMapRight = new BitMap32ID [ 4096 * 24 ];
  for ( int id = 0; id < 4096; ++ id )
  {
    all_cubeid( cubeID )
    {
      lowMapLeft  [ 24 * id + cubeID ] = MultipleRotation ( cubeID, id, trLow  );
      highMapLeft [ 24 * id + cubeID ] = MultipleRotation ( cubeID, id, trHigh );
      lowMapRight [ 24 * id + cubeID ] = MultipleRotation ( cubeID, id, trLow,  true );
      highMapRight[ 24 * id + cubeID ] = MultipleRotation ( cubeID, id, trHigh, true );
    }
  }
    m_lowMapLeft  = lowMapLeft;
    m_highMapLeft = highMapLeft;
    m_lowMapRight = lowMapRight;
    m_highMapRight= highMapRight;
}

void CubeSet::PrintHead()
{
  for ( int i = 0; i < 3; ++ i )
  {
    NL( Color::dark );
    all_cubeid( cid )
    {
      if ( cid % 4 == 0 )
      {
        slog_ ( "", Color::white, '|', Color::dark );
      }
      slog_( "", Simplex::GetCube( 23 - cid ).toChar( i ) );
    }
  }
  NL( Color::off );
}

void CubeSet::Print( BitMap32ID bid, const bool head )
{
  if ( head )
  {
    PrintHead();
  }
  BitMap::Print( bid, 24, 4 );
}


void CubeSet::Print_( BitMap32ID bid, const bool head )
{
  if ( head )
  {
    PrintHead();
  }
  BitMap::Print_( bid, 24, 4 );
}

CubeSet::~CubeSet()
{
  delete[] m_lowMapLeft;
  delete[] m_highMapLeft;
    m_lowMapLeft  = nullptr;
    m_highMapLeft = nullptr;
}
