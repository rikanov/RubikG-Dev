#ifndef FACTORY_STATE__TPP
#define FACTORY_STATE__TPP

#include <dynamic_array.h>
#include <factory_tree.h>

template< cube_size N >
class Factory<N>::StateFactory
{
  void init();
protected:
  static Array<BitMapID> AllowedRotations;
  StateFactory();
};

template< cube_size N >
Array<BitMapID> Factory<N>::StateFactory::AllowedRotations = Array<BitMapID>( CRotations<N>::AllRotIDs );

template< cube_size N >
Factory<N>::StateFactory::StateFactory()
{
  if ( 0 == AllowedRotations[0] )
  {
    init();
  }
}

template< cube_size N >
void Factory<N>::StateFactory::init()
{
  constexpr size_t AllRot = CRotations<N>::AllRotIDs;
  constexpr BitMapID allRotations = ( 1ULL << AllRot ) - 1; // include the solved bit

  RotID rotID = 0;
  BitMapID allow = allRotations;

  AllowedRotations[ rotID ++ ] = allRotations;

  all_rot( axis, layer, turn, N )
  {
    if ( 1 == turn && 0 == layer )
      allow = allRotations;

    if ( 1 == turn )
      allow -= 7ULL << ( 3 * ( axis * N + layer ) + 1 );

    AllowedRotations[ rotID ++ ] = allow;
  }
}

#endif  //  ! FACTORY_STATE__TPP
