#include <simplex.h>
#include <random>

Simplex * Simplex::Singleton = nullptr;

Simplex::Simplex()
{
  // Constructor body
  Singleton = this;
  init();
  initGroup();
}

void Simplex::init()
{
  // determine group IDs. Identity element most have ID = 0
  CubeID lastRadix[6] = {};
  all_orient ( right )
  {
    all_orient ( up )
    {
      if ( Coaxial(right, up) )
      {
        m_simplexGroupID [ right ][ up ] = _NF;
        continue;
      }
      CubeID front = OCube::FrontSide( right, up );
      const int index = 4 * front + lastRadix[ front ] ++;
      m_simplexGroupID [ right ][ up ] = index;
      m_simplexGroup [ index ].init ( right, up, index );
       // created all the 24 orientation of a simplex and store them in an array
      //  indexed by their group ID
    }
  }
}

void Simplex::initGroup()
{
  // init algebrian group
  all_cubeid ( a )
    all_cubeid ( b )
    {
      OCube A = getCube ( a );
      OCube B = getCube ( b );

      // create a composition of them:
      Orient cr = A.whatIs ( B.whatIs ( _R ) );
      Orient cu = A.whatIs ( B.whatIs ( _U ) );

      const CubeID c = GetGroupID ( cr, cu );

      m_composition [a][b] = c;
      m_transform   [a][c] = b;
    }

  // init tilts  
  m_tilt [ _X ] [1] = getGroupID( _R, _F );
  m_tilt [ _Y ] [1] = getGroupID( _B, _U );
  m_tilt [ _Z ] [1] = getGroupID( _U, _L );
  for ( Axis a: { _X, _Y, _Z } )
  {
    for ( int t: { 2, 3 } )
    {
      m_tilt [a][t] = composition( m_tilt [a][t-1], m_tilt [a][1] );
    }
  }
  all_cubeid( id )
  {
    for( Orient F: { _F, _R, _U, _L, _D, _B } )
      m_align[ id ][F] = GetCube( id ).aligned( F ); 
  }
}

CubeID Simplex::random() const
{
  static std::random_device                 randomDevice;
  static std::default_random_engine         randomEngine( randomDevice() );
  static std::uniform_int_distribution<int> distribution( 0, 23 );

  return distribution( randomEngine );
}
