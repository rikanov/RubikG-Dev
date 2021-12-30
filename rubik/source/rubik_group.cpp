#include <rubik_group.h>


template< cube_size N >
RubikGroup<N>::RubikGroup()
  : ArrayR ( CPositions<N>::AllPosIDs )
{
  init();
}

template< cube_size N >
void RubikGroup<N>::init()
{
  for( PosID pos = 0; pos < ArrayR::size(); ++ pos )
  {
    ArrayR::get( pos ).set( pos );
  }
}

template< cube_size N >
void RubikGroup<N>::rotate( const RotID rotID )
{
  const Layer layer = CRotations<N>::GetLayer( rotID );
  const size_t size = ( 0 == layer || N - 1 == layer ) ? N * N : 4 * ( N - 1 );
  size_t counter = 0;
  for( auto P = ArrayR::begin(); P != ArrayR::end(); ++ P )
  {
    counter += P -> rotate( rotID );
    if ( size == counter )
    {
      break;
    }
  }
  Sequence::push( rotID );
}

template<cube_size N> void RubikGroup<N>::rotate( const Sequence & seq )
{
  for( auto rot : seq )
  {
    rotate( rot );
  }
}

template< cube_size N >
CubeID RubikGroup<N>::getCubeID( const PosID pos ) const
{
  for( PosID posID = 0; pos < ArrayR:: size(); ++ posID )
  {
    if ( ArrayR::get( posID ).posID() == pos )
    {
      return ArrayR::get( posID ).state();
    }
  }
  clog( Color::red, "invalid position:", (int) pos, Color::off );
  return 0xFF;
}

template< cube_size N >
PosID RubikGroup<N>::whatIs( const PosID pos ) const
{
  for( PosID posID = 0; pos < ArrayR:: size(); ++ posID )
  {
    if ( ArrayR::get( posID ).posID() == pos )
    {
      return posID;
    }
  }
  clog( Color::red, "invalid position:", (int) pos, Color::off );
  return 0xFF;
}

template< cube_size N >
PosID RubikGroup<N>::whereIs( const PosID pos ) const
{
  return ArrayR::get( pos ).posID();
}

template< cube_size N >
CubeID RubikGroup<N>::transpose( const PosID pos, const CubeID trans) const
{
  const CubeID state = ArrayR::get( CPositions<N>::GetPosID( pos, trans ) ).state();
  return Simplex::Composition( trans, state );
}

template< cube_size N >
CubeID RubikGroup<N>::getPrior( const Pattern<N> & pattern, const CubeID trans ) const
{
  return transpose( pattern.getPriorPos(), trans );
}

template< cube_size N >
GroupID RubikGroup<N>::getState( const Pattern<N> & pattern, const CubeID trans ) const
{
  GroupID result = 0;
  const CubeID inv = Simplex::Inverse( getPrior( pattern, trans ) );
  for ( PosID posID = 0; posID < pattern.size() - 1; ++ posID )
  {
    const CubeID state = Simplex::Composition( transpose( pattern.get( posID ), trans ), inv );
    result += state * pow24( posID );
  }
  return result;
}

