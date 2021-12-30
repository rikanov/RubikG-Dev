#ifndef ___RUBIK_SLOT__H
#define ___RUBIK_SLOT__H

#include <base_types.h>
#include <bitmap_set.h>
#include <cube_rotations.h>
#include <cube_positions.h>

template< cube_size N >
class RubikSlot
{
  BitMap m_actOn;
  PosID  m_posID;
  CubeID m_state;

  void set();

public:
  RubikSlot();
  void set( const PosID );
  bool rotate( const RotID );
  CubeID state() const;
  PosID  posID() const;
};

// include template source file
#include "../source/rubik_slot.cpp"

#endif  //  ! ___RUBIK_SLOT__H
