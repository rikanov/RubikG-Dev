#ifndef FACTORY_STATE__TPP
#define FACTORY_STATE__TPP

template< cube_size N >
class Progress<N>::InitialState
{
  void init();
protected:
  static Array<BitMapID> AllowedRotations;
  InitialState();
};

template< cube_size N >
Array<BitMapID> Progress<N>::InitialState::AllowedRotations = Array<BitMapID>( CRotations<N>::AllRotIDs );

template< cube_size N >
Progress<N>::InitialState::InitialState()
{
  if ( 0 == AllowedRotations[0] )
  {
    init();
  }
}

template< cube_size N >
void Progress<N>::InitialState::init()
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

template< cube_size N >
class Progress<N>::StateAPI: public Progress<N>::InitialState
{

public:
  BitMap m_gradient;
  BitMap m_target;
  RotID  m_step;

  void initGradient( const RotID rotID )
  {
    m_gradient.set( InitialState::AllowedRotations[ rotID ] );
  }
};

#endif  //  ! FACTORY_STATE__TPP
