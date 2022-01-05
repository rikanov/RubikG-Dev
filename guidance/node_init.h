#ifndef ___NODE_INIT__H
#define ___NODE_INIT__H

#include <dynamic_array.h>
#include <symmetry.h>

template < cube_size N >
class NodeInit: protected Symmetry<N>
{
  Array<BitMapID>  m_allowedGradient;
  bool m_symmetricSearch;
  static constexpr BitMapID NoRestriction = ( 1ULL << CRotations<N>::AllRotIDs ) - 1;

protected:
  NodeInit();
  void initGradients( const size_t restriction = NoRestriction );

  void setAsRoot ( Node * node, const bool, const bool );
  bool setAsChild( Node * node, const bool ) const;
};

template< cube_size N >
NodeInit<N>::NodeInit()
  : m_allowedGradient( CRotations<N>::AllRotIDs )
{
  initGradients();
}

template< cube_size N >
void NodeInit<N>::setAsRoot( Node * node, const bool optional, const bool symmetry )
{
  m_symmetricSearch = symmetry;
  if ( ! optional )
  {
    node -> gradient = m_allowedGradient[0];
    node -> target   = ( 1 << 24 ) - 1;
  }
  if ( symmetry )
  {
    node -> asymmetry = 0;
    node -> reverseSteps = 0;
    node -> gradient.restrict( Symmetry<N>::Check( node ) );
  }
}

template< cube_size N >
bool NodeInit<N>::setAsChild( Node* node, const bool optional ) const
{
  bool result = true;
  if ( optional )
  {
    result = node -> gradient.restrict( m_allowedGradient[ ( node - 1 ) -> rotate ] ) &&
             node -> target.restrict( ( node - 1 ) -> target );
  }
  else
  {
    node -> gradient = m_allowedGradient[ ( node - 1 ) -> rotate ];
    node -> target   = ( node - 1 ) -> target;
  }
  if ( m_symmetricSearch )
  {
    const BitMapID check = Symmetry<N>::Check( node );
    node -> gradient.restrict( check );
  }
  return result;;
}


template< cube_size N >
void NodeInit<N>::initGradients( const size_t restriction )
{
  m_allowedGradient = Array<BitMapID>( CRotations<N>::AllRotIDs );
  RotID rotID    = 0;
  BitMapID allowed = restriction;

  m_allowedGradient[ rotID ++ ] = allowed;

  all_rot( axis, layer, turn, N )
  {
    if ( 1 == turn && 0 == layer )
      allowed = restriction;

    if ( 1 == turn )
    {
      const BitMapID mask = Symmetry<N>::Mask( axis, layer );
      allowed |= mask;
      allowed -= mask;
    }
    m_allowedGradient[ rotID ++ ] = allowed;
  }
}

#endif  //  ! ___NODE_INIT__H
