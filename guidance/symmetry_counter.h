#ifndef ___SYMMETRY_COUNTER__H
#define ___SYMMETRY_COUNTER__H

#include <dynamic_array.h>
#include <cube_rotations.h>
#include <node.h>

template< cube_size N >
class SymmetryCounter: public Array<Turn>
{
  size_t m_asymmetry;

  BitMapID symmetricStep( const RotID );
public:
  SymmetryCounter();
  BitMapID check( const Node * );
  
};

template< cube_size N >
SymmetryCounter::SymmetryCounter()
  : Array<Turn>( CRotations<N>::AllRotIDs / 3 )
{}

template< cube_size N >
BitMapID SymmetryCounter::check( const Node * node )
{
  const RotID rotID = node -> rotate;
  if ( node -> depth == m_asymmetry )
  {
    return symmetricStep( rotID );
  }
  const Turn  turn  = CRotations<N>::GetTurn( rotID );
  Turn & slot = Array<Turn>::get( ( rotID - 1 ) / 3 );
  m_asymmetry += 0 == slot;
  slot += turn;
  if ( slot > 3 )
  {
    slot -= 4;
    m_asymmetry -= 0 == slot;
  }
  
}
#endif  //  ! ___SYMMETRY_COUNTER__H