#ifndef RAW_MAP__H
#define RAW_MAP__H

#include <cube_positions.h>
#include <cube_rotations.h>
#include <rubik.h>
#include <state_printer.h>

typedef uint64_t GroupID;
typedef uint32_t SmallGroupID;

template< cube_size N >
class Subgroup
{
  size_t         m_size;
  GroupID        m_stateID;
  GroupID      * m_singleCache;
  SmallGroupID * m_monoCache;
  
  const PosID * m_startPos;
  
  void add  ( const PosID );

public:

  Subgroup();
  Subgroup( const PosID *, const size_t size, const CubeID orient = 0 );
  ~Subgroup();
  
  GroupID check( const RotID, const bool prior = false ) const;
  
  void buildCache();
  
  void init( const PosID *, const size_t, const CubeID );
  
  void move( const RotID rotID )
  {
    m_stateID = check( rotID );
  }
  
  GroupID state () const
  {
    return m_stateID;
  }
  
  size_t size() const
  {
    return m_size;
  }
  
  void print( const bool details = false ) const
  {
    PrintMap<N> ( m_stateID, m_startPos, m_size, details );
  }
};

template< cube_size N >
Subgroup<N>::Subgroup()
 :  m_size        ( 0 )
 ,  m_stateID     ( 0 )
 ,  m_singleCache ( nullptr )
 ,  m_monoCache   ( nullptr )
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
  if ( size != m_size )
  {
    delete[] m_singleCache;
    m_singleCache = new GroupID [ size * 24 * CRotations<N>::AllRotIDs ];   
  }
  delete[] m_monoCache; m_monoCache = nullptr;
  m_startPos = pos;
  
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
        m_singleCache[ offset + base + rotID ] = Simplex::Composition( cubeID, Simplex::Tilt( axis, turn ) ) * pow24( m_size );
      }
      else 
      {
        m_singleCache[ offset + base + rotID ] = cubeID * pow24( m_size );
      }
    }
  }
  ++ m_size;
}

template< cube_size N >
GroupID Subgroup<N>::check( const RotID rotID, const bool prior ) const
{
  if ( m_monoCache )
  {
    return m_monoCache[ m_stateID * CRotations<N>::AllRotIDs + rotID ];
  }
  GroupID result = 0;
  for( GroupID stateID = m_stateID, offset = 0; offset < m_size - prior; stateID /= 24, ++ offset )
  {
    result += m_singleCache[ ( offset * 24 + stateID % 24 ) * CRotations<N>::AllRotIDs + rotID ];
  }
  return result;
}

template< cube_size N >
void Subgroup<N>::buildCache()
{
  delete[] m_monoCache;
  m_monoCache = new SmallGroupID[ pow24( m_size ) * CRotations<N>::AllRotIDs ];
}

template< cube_size N >
Subgroup<N>::~Subgroup()
{
  delete[] m_singleCache;
  delete[] m_monoCache;
}

#endif  //  ! RAW_MAP__H
