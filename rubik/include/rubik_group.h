#ifndef ___RUBIK_GROUP__H
#define ___RUBIK_GROUP__H

#include <pattern.h>
#include <sequence.h>
#include <rubik_slot.h>

template< cube_size N >
class RubikGroup: public Array< RubikSlot<N> >
                , public Sequence
{
  using ArrayR = Array< RubikSlot<N> >;
  void init();

public:
  RubikGroup();
  void rotate( const RotID );
  void rotate( const Sequence & );

  // Query functions
  CubeID  getCubeID ( const PosID ) const;

  PosID whatIs  ( const PosID ) const;
  PosID whereIs ( const PosID ) const;

  CubeID transpose ( PosID id, CubeID trans = 0 ) const ;

  CubeID  getPrior( const Pattern<N> &, const CubeID ) const;
  GroupID getState( const Pattern<N> &, const CubeID ) const;
};

// include template source file
#include "../source/rubik_group.cpp"

#endif  //  ! ___RUBIK_GROUP__H
