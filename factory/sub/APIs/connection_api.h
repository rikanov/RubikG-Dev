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
  ConnectionAPI( const size_t size, const PosID * pos );

  void connectIfChild( NodeChart & , const NodeChart &, const RotID ) const;
  void connectOnSameLevel( NodeChart & , NodeChart &, const RotID ) const ;
  void connectAimsToParent( NodeChart & , const NodeChart & , const RotID ) const;
};


template< cube_size N >
GuideFactory<N>::ConnectionAPI::ConnectionAPI()
  : Connection()
{
}

template< cube_size N >
GuideFactory<N>::ConnectionAPI::ConnectionAPI( const size_t size, const PosID * pos )
  : Connection( size, pos )
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
    child.grade[0] |= 1ULL << rotInv;
    child.aim[0]   |= mergeAim( rotInv, parent.aim[0] );
  }
}

template< cube_size N >
void GuideFactory<N>::ConnectionAPI::connectOnSameLevel( NodeChart & node, NodeChart & neighbor, const RotID rotID ) const
{
  if ( node.level == neighbor.level )
  {
    node.grade[1] |= ( 1ULL << rotID );
    node.aim  [1] |= mergeAim( rotID, neighbor.aim[0] );
  }
}

template< cube_size N >
void GuideFactory<N>::ConnectionAPI::connectAimsToParent( NodeChart & child, const NodeChart & parent, const RotID rotID ) const
{
  const RotID inv = CRotations<N>::GetInvRotID( rotID );
  child.aim[0] |= mergeAim( inv, parent.aim[0] );
  child.aim[1] |= mergeAim( inv, parent.aim[1] );
}

#endif  //  ! ___CONNECTION_RULES_API__H
