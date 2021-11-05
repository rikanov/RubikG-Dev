#ifndef CONNECTION_RULES__H
#define CONNECTION_RULES__H

template< cube_size N >
class ConnectionsAPI
{
  pArray<CubeID> m_priorPos;
  
  inline BitMap32ID mergeAim( const RotID rotID, const BitMap32ID set ) const;

public:
  void init( pArray<CubeID> p);
  void connectIfChild( NodeChart & , const NodeChart &, const RotID ) const;
  void connectOnSameLevel( NodeChart & , NodeChart &, const RotID ) const ;
  void connectAimsToParent( NodeChart & , const NodeChart & , const RotID ) const;
};

template< cube_size N >
void ConnectionsAPI<N>::init( pArray<CubeID> p)
{
  m_priorPos = p;
}

template< cube_size N >
BitMap32ID ConnectionsAPI<N>::mergeAim( const RotID rotID, const BitMap32ID set ) const
{
  return m_priorPos -> value & ( 1ULL << rotID ) ? CubeSet::GetCubeSet( CRotations<N>::GetTilt( rotID ), set ) : set;
}

template< cube_size N >
void ConnectionsAPI<N>::connectIfChild( NodeChart & child, const NodeChart & parent, const RotID rotID ) const
{
  if ( child.value == parent.value + 1 )
  {
    const RotID rotInv = CRotations<N>::GetInvRotID( rotID );
    child.grade[0] |= 1ULL << rotInv;
    child.aim[0]   |= mergeAim( rotInv, parent.aim[0] );
  }
}

template< cube_size N >
void ConnectionsAPI<N>::connectOnSameLevel( NodeChart & node, NodeChart & neighbor, const RotID rotID ) const 
{
  if ( node.value == neighbor.value )
  {
    node.grade[1] |= ( 1ULL << rotID );
    node.aim  [1] |= mergeAim( rotID, neighbor.aim[0] );
  }
}

template< cube_size N >
void ConnectionsAPI<N>::connectAimsToParent( NodeChart & child, const NodeChart & parent, const RotID rotID ) const
{
  const RotID inv = CRotations<N>::GetInvRotID( rotID );
  child.aim[0] |= mergeAim( inv, parent.aim[0] );
  child.aim[1] |= mergeAim( inv, parent.aim[1] );
}

#endif  //  ! CONNECTION_RULES__H