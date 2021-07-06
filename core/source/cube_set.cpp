#include <cube_set.h>
#include <bitmap_set.h>
#include <simplex.h>

CubeSet * CubeSet::Singleton = nullptr;
const BitMap32ID * CubeSet::m_lowMap  = nullptr;
const BitMap32ID * CubeSet::m_highMap = nullptr;

static BitMap32ID MultipleRotation( const CubeID state, BitMap32ID cubeSet, const CubeSet::TransMode tr = CubeSet::trNormal  )
{
  BitMap32ID result = 0;
  if( tr == CubeSet::trHigh )
  {
    cubeSet <<= 12;
  }
  BitMap operations( cubeSet );
  CubeID next;
  while ( operations >> next )
  {
    result |= ( 1 << Simplex::Composition( state, next ) );
  }
  return result;
}

CubeSet::CubeSet()
{
  BitMap32ID * lowMap  = new BitMap32ID [ 4096 * 24 ];
  BitMap32ID * highMap = new BitMap32ID [ 4096 * 24 ];
  
  for ( int id = 0; id < 4096; ++ id )
    all_cubeid( cubeID )
    {
      lowMap  [ 24 * id + cubeID ] = MultipleRotation ( id, cubeID, trLow  );
      highMap [ 24 * id + cubeID ] = MultipleRotation ( id, cubeID, trHigh );
    }
  
  m_lowMap  = lowMap;
  m_highMap = highMap;
}

CubeSet::~CubeSet()
{
  delete[] m_lowMap;
  delete[] m_highMap;
  m_lowMap  = nullptr;
  m_highMap = nullptr;
}
