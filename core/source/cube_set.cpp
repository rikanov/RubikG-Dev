#include <cube_set.h>
#include <simplex.h>

CubeSet * CubeSet::Singleton = nullptr;
const CubeSetID * CubeSet::m_lowMap  = nullptr;
const CubeSetID * CubeSet::m_highMap = nullptr;

static CubeSetID TransformSolution( const CubeSetID sid, const CubeID state, const CubeSet::TransMode tr = CubeSet::trNormal )
{
  CubeSetID result = 0;
  CubeSetID mask   = 1;
  for( int id = tr == CubeSet::trHigh ? 12: 0; id < ( ( tr == CubeSet::trLow ) ? 12 : 24 ); ++ id, mask <<= 1 )
  {
    if ( sid & mask )
	  result |= ( 1 << Simplex::Composition( state, id ) );
  }
  return result;
}

CubeSet::CubeSet()
{
  CubeSetID * lowMap  = new CubeSetID [ 4096 * 24 ];
  CubeSetID * highMap = new CubeSetID [ 4096 * 24 ];
  
  for ( int id = 0; id < 4096; ++ id )
    all_cubeid( cubeID )
    {
      lowMap  [ 24 * id + cubeID ] = TransformSolution( id, cubeID, trLow  );
      highMap [ 24 * id + cubeID ] = TransformSolution( id, cubeID, trHigh );
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
