#ifndef ___NODE_INIT__H
#define ___NODE_INIT__H

#include <dynamic_array.h>
#include <cube_rotations.h>
#include <node.h>

template < cube_size N >
class NodeInit
{
  Array<BitMapID>  m_allowedGradient;
  void initGradients();

protected:
  NodeInit();
  ~NodeInit();

  void setRoot( Node * node ) const;
  void nextNode( Node * node, const RotID rotID ) const;
};

template< cube_size N >
NodeInit<N>::NodeInit()
  : m_allowedGradient( CRotations<N>::AllRotIDs )
{
  initGradients();
}

template< cube_size N >
void NodeInit<N>::setRoot( Node* node ) const
{
  node -> gradient = m_allowedGradient[0];
  node -> target   = ( 1 << 24 ) - 1;
}

template< cube_size N >
void NodeInit<N>::nextNode( Node* node, const RotID rotID ) const
{
  ( node + 1 ) -> gradient = m_allowedGradient[ rotID ];
  ( node + 1 ) -> target   = node -> target;
}


template< cube_size N >
void NodeInit<N>::initGradients()
{
  constexpr BitMapID allRotations = ( 1ULL << CRotations<N>::AllRotIDs ) - 1; // include the solved bit

  RotID rotID    = 0;
  BitMapID allow = allRotations;

  m_allowedGradient[ rotID ++ ] = allRotations;

  all_rot( axis, layer, turn, N )
  {
    if ( 1 == turn && 0 == layer )
      allow = allRotations;

    if ( 1 == turn )
      allow -= 7ULL << ( 3 * ( axis * N + layer ) + 1 );

    m_allowedGradient[ rotID ++ ] = allow;
  }
}

template< cube_size N >
NodeInit<N>::~NodeInit()
{
}

#endif  //  ! ___NODE_INIT__H
