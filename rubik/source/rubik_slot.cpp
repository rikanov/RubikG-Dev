#include <rubik_slot.h>


template< cube_size N >
RubikSlot<N>::RubikSlot()
  : m_actOn( 0 )
  , m_posID( 0 )
  , m_state( 0 )
{
}

template< cube_size N >
void RubikSlot<N>::set()
{
  m_actOn = CPositions<N>::ActOn( CPositions<N>::GetPosID( m_posID, m_state ) );
}

template<cube_size N> void RubikSlot<N>::set( const PosID posID )
{
  m_posID = posID;
  m_state = 0;
  set();
}

template< cube_size N >
bool RubikSlot<N>::rotate( const RotID rotID )
{
  if ( m_actOn.contains( rotID ) )
  {
    m_state = CRotations<N>::Tilt( m_state, rotID );
    set();
    return true;
  }
  return false;
}

template< cube_size N >
CubeID RubikSlot<N>::state() const
{
  return m_state;
}

template< cube_size N >
PosID RubikSlot<N>::posID() const
{
  return CPositions<N>::GetPosID( m_posID, m_state );
}
