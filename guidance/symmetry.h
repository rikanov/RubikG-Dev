#ifndef ___SYMMETRY_COUNTER__H
#define ___SYMMETRY_COUNTER__H

#include <bitmap_set.h>
#include <cube_rotations.h>
#include <node.h>

template< cube_size N >
class Symmetry
{
  static constexpr BitMapID NoRestriction = ( 1ULL << CRotations<N>::AllRotIDs ) - 1;

  static void SetNextNode( Node * );
  static BitMapID SameLayersAs( BitMap rotSet );
protected:
  static BitMapID Mask( const RotID );
  static BitMapID Mask( const Axis, const Layer );
  static bool Force( Node * );

};

template< cube_size N > inline
BitMapID Symmetry<N>::Mask( const RotID rotID )
{
  return 7ULL << ( rotID - CRotations<N>::GetTurn( rotID ) + 1 );
}

template< cube_size N > inline
BitMapID Symmetry<N>::Mask( const Axis axis, const Layer layer )
{
  return 7ULL << ( 3 * ( axis * N + layer ) + 1 );
}

template< cube_size N > inline
bool Symmetry<N>::Force( Node * node )
{
  SetNextNode( node );
  switch ( node -> depth - node -> asymmetry )
  {
    case 0:
      node -> gradient &= node -> reverseSteps;
      break;
    case 1:
      node -> gradient &= SameLayersAs( node -> reverseSteps );
      break;
    default:
      ;
  }
  return ! node -> gradient.empty();
}

template< cube_size N > inline
void Symmetry<N>::SetNextNode( Node * node )
{
  const Node * prev = node - 1;
  const RotID rotID = CRotations<N>::GetInvRotID( prev -> rotate );
  const BitMapID mask = Mask( rotID );

  BitMap inverse;
  if ( ! prev -> reverseSteps.hasCommon( mask ) )
  {
    node -> asymmetry = prev -> asymmetry + 1;
    inverse.add( rotID );
  }
  else
  {
    // 4-cyclic Abelian group represented by bitwise operations in rotation id map
    inverse.set( ( ( prev -> reverseSteps & mask) ) << CRotations<N>::GetTurn( rotID ) );
    inverse.expand( inverse >> 4 );
    inverse.restrict( mask );
    node -> asymmetry = prev -> asymmetry - ( inverse == 0 );
  }
  // set next node reverseSteps by the new inverse step
  node -> reverseSteps = inverse | ( prev -> reverseSteps & ~mask );
}

template< cube_size N > inline
BitMapID Symmetry<N>::SameLayersAs( BitMap rotSet )
{
  rotSet.remove( 0 );
  BitMapID result = 0;
  for ( RotID inv = 0; rotSet >> inv; )
  {
    result |= Mask( inv );
  }
  return result;
}
#endif  //  ! ___SYMMETRY_COUNTER__H
