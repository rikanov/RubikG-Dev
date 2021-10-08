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
  const GroupID  * m_singleCache;
  
  void addCube( GroupID *, const size_t );
  void initSimplices();
  void initCache();

  void copyLine( const GroupID *, GroupID *       );
  void addLines( GroupID *      , const GroupID * );
  
  void copyBlocks     ( const size_t, GroupID * );
  void increaseBlocks ( const size_t, GroupID * );

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
    return m_startPositions[id];
  }

  GroupID lookUp( const GroupID gid, const RotID rid ) const
  {
    return m_projectedGroupID[ AllRot * gid + rid ];
  }
  
  GroupID lookUp( const GroupID gid, const RotID rid, const CubeID cid ) const
  {
    const RotID trans = CRotations<N>::GetRotID( rid, Simplex::Inverse( cid ) );
    return lookUp( gid, trans );
  }
  
  CubeID  getPrior  ( const Rubik<N> &, const CubeID trans ) const;
  GroupID getStateID( const Rubik<N> &, const CubeID trans ) const;
  
  void print( const GroupID gid, const CubeID prior, const bool details = false ) const
  {
    const GroupID stateID = prior * pow24( m_size - 1 ) + gid;
    clog_( stateID );
    PrintMap<N> ( stateID, m_startPositions, m_size, details );    
  }
};

template< cube_size N >
Subgroup2<N>::Subgroup2( const PosID * pos, const size_t size, const CubeID orient )
 :  m_size( size )
 ,  m_priorRotIDs( CRotations<N>::ActOn( pos[ size - 1 ] ) )
 ,  m_startPositions   ( nullptr )
 ,  m_projectedGroupID ( nullptr )
 ,  m_singleCache      ( nullptr )
{
  PosID * startPos = new PosID[ size ] {};  
  for( size_t s = 0; s < size; ++ s )
  {
    startPos[s] = CPositions<N>::GetPosID( pos[s], orient );
  }
  m_startPositions = startPos;
  
  initCache();
  BitMap::Print ( m_priorRotIDs, 9 * N + 1, 3 * N );
}

template< cube_size N >
void Subgroup2<N>::addCube( GroupID * simplex, const size_t pos )
{
  all_cubeid ( cid )
  {
    RotID rid = 0;
    all_rot( axis, layer, turn, N )
    {
      ++ rid;
      CubeID res;
      if ( layer == CPositions<N>::GetLayer( positions( pos ), cid, axis ) )
      {
        res = Simplex::Tilt( cid, axis, turn );
      }
      else
      {
        res = cid;
      }
      
      if ( m_priorRotIDs & ( 1ULL << rid ) )
      {
        res = Simplex::Tilt( res, axis, 4 - turn );
      }
      simplex[ ( 24 * pos + cid ) * AllRot + rid ] = res * pow24( pos );
    }
  }
}

template< cube_size N >
void Subgroup2<N>::initSimplices()
{
  GroupID * simplex = new GroupID [ AllRot * 24 * m_size ] {};
  for ( size_t pos = 0; pos < m_size; ++ pos )
  {
    addCube( simplex, pos );
  }
  m_singleCache = simplex;
}

template< cube_size N >
void Subgroup2<N>::copyLine( const GroupID * from, GroupID * to )
{
  all_rotid( rotID, N )
  {
    *( to + rotID ) = *( from + rotID );
  }
}

template< cube_size N >
void Subgroup2<N>::addLines( GroupID * base, const GroupID * add )
{
  all_rotid( rotID, N )
  {
    *( base + rotID ) += *( add + rotID );
  }
}

template< cube_size N >
void Subgroup2<N>::copyBlocks( const size_t pow, GroupID * cache )
{
  GroupID next = 0;
  all_cubeid( block )
  {
    for ( size_t line = 0; line < pow24( pow ); ++ line, ++ next )
    {
      cache[ next * AllRot ] = next;
      copyLine( cache + line * AllRot, cache + next * AllRot );
    }
  }
}

template< cube_size N >
void Subgroup2<N>::increaseBlocks( const size_t pow, GroupID * cache )
{
  GroupID next = 0;
  all_cubeid( block )
  {
    const GroupID * add = m_singleCache + ( 24 * pow + block ) * AllRot;
    for ( size_t line = 0; line < pow24( pow ); ++ line, ++ next )
    {
      addLines( cache + next * AllRot, add );
    }
  }
}
template< cube_size N >
void Subgroup2<N>::initCache()
{
  initSimplices();
  GroupID * cache = new GroupID [ pow24( m_size - 1 ) * AllRot  + 1 ] {};
  for ( size_t pos = 0; pos < m_size - 1; ++ pos )
  {
    copyBlocks     ( pos, cache );
    increaseBlocks ( pos, cache );
  }
  m_projectedGroupID = cache;
}

template< cube_size N >
CubeID Subgroup2<N>::getPrior( const Rubik<N> & Cube, const CubeID trans ) const
{
  return Cube.transpose( m_startPositions[ m_size - 1 ], trans );
}

template< cube_size N >
GroupID Subgroup2<N>::getStateID( const Rubik<N> & Cube, const CubeID trans ) const
{
  BitMapID result = 0;
  for ( int id = 0; id < m_size - 1; ++ id )
  {
    result += Cube.transpose( m_startPositions[id], trans ) * pow24( id );
  }
  return result;
}

template< cube_size N >
Subgroup2<N>::~Subgroup2()
{
  delete[] m_startPositions;
  delete[] m_projectedGroupID;
  delete[] m_singleCache;
}

#endif  //  ! SUBGROUP_CACHED__H