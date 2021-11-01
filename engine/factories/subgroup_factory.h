#ifndef SUBGROUP_FACTORY
#define SUBGROUP_FACTORY

#include <cube_positions.h>
#include <cube_rotations.h>
#include <smart_array.h>
#include <subgroup.h>

template< cube_size N >
class FSubgroup
{
  static constexpr size_t AllRot = CRotations<N>::AllRotIDs;

  size_t        m_size;
  BitMapID      m_priorRotIDs;
  const PosID * m_position;

  void addCube( GroupID *, const size_t );
  void copyLine( const GroupID *, GroupID * );
  void addLines( GroupID *, const GroupID * );

  void copyBlocks     ( const size_t, GroupID * );
  void increaseBlocks ( const size_t, GroupID *, const GroupID * );

  const GroupID * getGroupCache();

public:
  FSubgroup();
  ~FSubgroup() ;

  pArray< GroupID > create( pArray< PosID > patch );
};

template< cube_size N >
FSubgroup<N>::FSubgroup()
{

}

template< cube_size N >
pArray< GroupID > FSubgroup<N>::create( pArray< PosID > patch )
{
  m_size     = patch -> size;
  m_position = patch -> data;

  m_priorRotIDs = CRotations<N>::ActOn( m_position[ m_size - 1 ] );
  return std::make_shared< Array< GroupID > > ( pow24( m_size - 1 ) + 1, getGroupCache(), m_priorRotIDs );
}

template< cube_size N >
void FSubgroup<N>::addCube( GroupID * simplex, const size_t pos )
{
  all_cubeid ( cid )
  {
    RotID rid = 0;
    all_rot( axis, layer, turn, N )
    {
      ++ rid;
      CubeID res;
      if ( layer == CPositions<N>::GetLayer( m_position[ pos ], cid, axis ) )
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
void FSubgroup<N>::copyLine( const GroupID * from, GroupID * to )
{
  all_rotid( rotID, N )
  {
    *( to + rotID ) = *( from + rotID );
  }
}

template< cube_size N >
void FSubgroup<N>::addLines( GroupID * base, const GroupID * add )
{
  all_rotid( rotID, N )
  {
    *( base + rotID ) += *( add + rotID );
  }
}

template< cube_size N >
void FSubgroup<N>::copyBlocks( const size_t pow, GroupID * cache )
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
void FSubgroup<N>::increaseBlocks( const size_t pow, GroupID * cache, const GroupID * simp )
{
  GroupID next = 0;
  all_cubeid( block )
  {
    const GroupID * add = simp + ( 24 * pow + block ) * AllRot;
    for ( size_t line = 0; line < pow24( pow ); ++ line, ++ next )
    {
      addLines( cache + next * AllRot, add );
    }
  }
}
template< cube_size N >
const GroupID * FSubgroup<N>::getGroupCache()
{
  GroupID * simplex = new GroupID [ AllRot * 24 * m_size ] {};
  for ( size_t pos = 0; pos < m_size; ++ pos )
  {
    addCube( simplex, pos );
  }
  GroupID * cache = new GroupID [ pow24( m_size - 1 ) * AllRot  + 1 ] {};
  for ( size_t pos = 0; pos < m_size - 1; ++ pos )
  {
    copyBlocks     ( pos, cache );
    increaseBlocks ( pos, cache, simplex );
  }
  delete[] simplex;
  return cache;
}

template< cube_size N >
FSubgroup<N>::~FSubgroup()
{
}

#endif  //  ! SUBGROUP_FACTORY
