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
  static BitMapID Check( Node * );

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

template< cube_size N >
BitMapID Symmetry<N>::Check( Node * node )
{
  SetNextNode( node );
  switch ( node -> depth - node -> asymmetry )
  {
    case 0:
      return node -> reverseSteps;
    case 1:
      return SameLayersAs( node -> reverseSteps );
    default:
      return NoRestriction;
  }
}

template< cube_size N > inline
void Symmetry<N>::SetNextNode( Node * node )
{
  const RotID rotID = CRotations<N>::GetInvRotID( node -> rotate );
  const BitMapID mask = Mask( rotID );

  Node * next = node + 1;
  BitMap inverse;
  if ( ! node -> reverseSteps.hasCommon( mask ) )
  {
    next -> asymmetry = node -> asymmetry + 1;
    inverse.add( rotID );
  }
  else
  {
    // 4-cyclic Abelian group represented by bitwise operations in rotation id map
    inverse.set( ( ( node -> reverseSteps & mask) ) << CRotations<N>::GetTurn( rotID ) );
    inverse.expand( inverse >> 4 );
    inverse.restrict( mask );
    next -> asymmetry = node -> asymmetry - ( inverse == 0 );
  }
  // set next node reverseSteps by the new inverse step
  BitMap & reverseSteps = next -> reverseSteps;
  reverseSteps = node -> reverseSteps;
  reverseSteps.restrict( ~mask );
  reverseSteps.expand( inverse );
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
