#ifndef BITMANIP__H
#define BITMANIP__H

#include <base_types.h>
#include <simplex.h>

template< cube_size N >
class GenerateRotationSet
{
  static constexpr unsigned m_length  = 3 * N;
  static constexpr BitMapID m_mask    = ( 1ULL << m_length ) - 1;

  static const BitMapID * m_flipped;
  static const BitMapID   * m_shadowing;

  GenerateRotationSet() = default;
  
  static void FlipAxis  ( BitMapID & , const Axis );
  static void Permute   ( BitMapID & , const Axis, const Axis, const Axis );
  static Axis TransAxis ( BitMapID & , const Axis, const CubeID );

  static void createFlipCache();
  static void createShadowing();

public:
 
  static void Instance();
  static void OnExit();
  static void Transform( BitMapID &, const CubeID );
};

// include template source
#include "../source/gen_rotations.cpp"

#endif  //  ! BITMANIP__H
