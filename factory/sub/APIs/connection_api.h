#ifndef ___CONNECTION_RULES_API__H
#define ___CONNECTION_RULES_API__H

#include <cube_set.h>
#include <factories/connection_factory.h>

template< cube_size N >
class GuideFactory<N>::ConnectionAPI: public GuideFactory<N>::Connection
{

  inline BitMap32ID mergeAim( const RotID rotID, const BitMap32ID set ) const;

public:
  ConnectionAPI();
  ConnectionAPI( const Connection & cr );
  ConnectionAPI( Pattern<N> pattern );

  void connectIfChild( NodeChart & , const NodeChart &, const RotID ) const;
};


template< cube_size N >
GuideFactory<N>::ConnectionAPI::ConnectionAPI()
  : Connection()
{
}

template< cube_size N >
GuideFactory<N>::ConnectionAPI::ConnectionAPI( Pattern<N> pattern )
  : Connection( pattern )
{
}
template< cube_size N >
BitMap32ID GuideFactory<N>::ConnectionAPI::mergeAim( const RotID rotID, const BitMap32ID set ) const
{
  return CubeSet::GetCubeSet( GuideFactory<N>::Connection::m_rotatePriorCube[ rotID ], set );
}

template< cube_size N >
void GuideFactory<N>::ConnectionAPI::connectIfChild( NodeChart & child, const NodeChart & parent, const RotID rotID ) const
{
  if ( child.level == parent.level + 1 )
  {
    const RotID rotInv = CRotations<N>::GetInvRotID( rotID );
    child.grade |= 1ULL << rotInv;
    child.aim   |= mergeAim( rotInv, parent.aim );
  }
}

#endif  //  ! ___CONNECTION_RULES_API__H
