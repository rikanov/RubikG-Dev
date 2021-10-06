#ifndef SUBGROUP_CACHED__H
#define SUBGROUP_CACHED__H

#include <cube_positions.h>
#include <cube_rotations.h>
#include <rubik.h>
#include <state_printer.h>

template< cube_size N >
class Subgroup2
{
  static constexpr size_t AllRot = CRotations<N>::AllRotIDs;
  
  const size_t     m_size;
  const BitMapID   m_priorRotIDs;
  const PosID    * m_startPositions;
  const GroupID  * m_projectedGroupID;
  const GroupID  * m_simplexGroupID;
  
  void initSimplices();
  void initCache();

public:
  Subgroup2( const PosID *, const size_t size, const CubeID orient = 0 );
  ~Subgroup2();

  size_t size() const
  {
    return m_size;
  }

  BitMapID priorRotIDs() const
  {
    return m_priorRotIDs;
  }

  PosID positions( const size_t id ) const
  {
    return m_startPos[id];
  }

  BitMapID getStateID( const Rubik<N> &, const CubeID trans ) const;
  
  void print( const GroupID stateID, const bool details = false, const bool projected = false ) const;
};

template< cube_size N >
Subgroup2<N>::Subgroup2( const PosID * pos, const size_t size, const CubeID orient = 0 )
 :  m_size( size )
 ,  m_priorRotIDs( CRotations<N>::ActOn( pos[ size - 1 ] ) )
 ,  m_startPositions   ( nullptr )
 ,  m_projectedGroupID ( nullptr )
 ,  m_simplexGroupID   ( nullptr )
{
  PosID * startPos = new PosID[ size ] {};  
  for( size_t s = 0; s < size; ++ s )
  {
    startPos[s] = CPositions<N>::GetPosID( pos[s], orient );
  }
  m_startPositions = startPos;
  
  initCache();
}


template< cube_size N >
void Subgroup2<N>::addCube( GroupID * simplex, const size_t pos )
{
  all_rotid ( rotID, N )
  {
    const bool   movingPrior = m_priorRotIDs & rotID; 
    const CubeID acting      = CRotations<N>::GetTilt( rotID );
    const CubeID priorInv    = movingPrior ? Simplex::Inverse( acting ) : 0;
    
    all_cubeid ( cid )
    {
      const bool   moving  = CRotations<N>::ActOn( m_startPositions[ pos ], cid ) & rotID;
      const CubeID rotated = moving ? Simplex::Composition( cid, acting ) : cid;
      const size_t memPos  = AllRot * ( 24 * pos + cid ) + rotID;
      
      simplex[ memPos ] = Simplex::Composition( rotated, priorInv ) * pow24( pos );
    }  
  }
}

template< cube_size N >
void Subgroup2<N>::initSimplices()
{
  GroupID * simplex = new GroupID [ AllRot * 24 * m_size ] {};
  for ( size_t pos = 0; pos < m_size )
  {
    addCube( simplex, pos );
  }
  m_simplexGroupID = simplex;
}

template< cube_size N >
void Subgroup2<N>::initCache()
{
  GroupID * cache = new GroupID [ pow24( m_size - 1 ) * AllRot  + 1 ] {};
  GroupID next = 0;
  for ( size_t pos = 0; pos < m_size - 1; ++ pos )
  {
    for ( CubeID cid = 1; cid < 24; ++ cid )
    {
      const GroupID * add = m_simplexGroupID + AllRot * ( 24 * pos + cid );
      for ( GroupID copy = 0; copy < pow24( pos ) * AllRot; ++ copy, ++ next )
      {
        m_projectedGroupID[ next ] = m_projectedGroupID[ copy ] + add[ next % AllRot ];
      }
    }
  }
  m_projectedGroupID = cache;
}


template< cube_size N >
Subgroup2<N>::~Subgroup2()
{
  delete[] m_startPositions;
  delete[] m_projectedGroupID;
  delete[] m_simplexGroupID;
}

#endif  //  ! SUBGROUP_CACHED__H