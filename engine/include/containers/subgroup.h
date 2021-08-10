#ifndef RAW_MAP__H
#define RAW_MAP__H

#include <cube_positions.h>
#include <cube_rotations.h>
#include <rubik.h>
#include <state_printer.h>

typedef uint64_t GroupID;

template< cube_size N >
class Subgroup
{
  size_t    m_size;
  GroupID   m_stateID;
  GroupID * m_cacheSingle;
  
  const PosID * m_startPos;
  
  void add  ( const PosID );

public:

  Subgroup();
  Subgroup( const PosID *, const size_t size, const CubeID orient = 0 );
  
  void    init  ( const PosID *, const size_t, const CubeID );
  void    move  ( const RotID );
  GroupID check ( const RotID ) const;
  GroupID state () const;
  
  void print( const bool details = false ) const
  {
    PrintMap<N> ( m_stateID, m_startPos, m_size, details );
  }
};

template< cube_size N >
Subgroup<N>::Subgroup()
 :  m_size        ( 0 )
 ,  m_stateID     ( 0 )
 ,  m_cacheSingle ( nullptr )
{
   
}

template< cube_size N >
Subgroup<N>::Subgroup( const PosID * pos, const size_t size, const CubeID orient )
 :  Subgroup()
{
  init( pos, size, orient );
}

template< cube_size N >
void Subgroup<N>::init ( const PosID * pos, const size_t size, const CubeID orient )
{
  delete[] m_cacheSingle;
  m_startPos = pos;
  m_cacheSingle = new GroupID [ size * 24 * CRotations<N>::AllRotIDs ];
  
  for( size_t s = 0; s < size; ++ s )
    add( CPositions<N>::GetPosID( *( pos ++ ), orient ) );
}

template< cube_size N >
void Subgroup<N>::add( const PosID pos )
{
  const size_t offset = m_size * 24 * CRotations<N>::AllRotIDs;
  
  all_cubeid( cubeID )
  {
    RotID rotID = 0;
    const PosID whereIs = CPositions<N>::GetPosID( pos, cubeID );
    const size_t base = cubeID * CRotations<N>::AllRotIDs;
    all_rot( axis, layer, turn, N )
    {
      ++ rotID;
      if ( CPositions<N>::GetLayer( whereIs , axis ) == layer )
      {
        m_cacheSingle[ offset + base + rotID ] = Simplex::Composition( cubeID, Simplex::Tilt( axis, turn ) ) * pow24( m_size );
      }
      else 
      {
        m_cacheSingle[ offset + base + rotID ] = cubeID * pow24( m_size );
      }
    }
  }
  ++ m_size;
}

template< cube_size N >
GroupID Subgroup<N>::check( const RotID rotID ) const
{
  GroupID result = 0;
  for( GroupID stateID = m_stateID, offset = 0; 0 < stateID; stateID /= 24, offset += 24 )
  {
    result += m_cacheSingle[ ( offset + stateID % 24 ) * CRotations<N>::AllRotIDs + rotID ];
  }
  return result;
}

template< cube_size N >
GroupID Subgroup<N>::state() const
{
  return m_stateID;
}

template< cube_size N >
void Subgroup<N>::move( const RotID rotID )
{
  m_stateID = check( rotID );
}
#endif  //  ! RAW_MAP__H
