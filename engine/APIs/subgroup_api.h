#ifndef SUBGROUP_CACHED__H
#define SUBGROUP_CACHED__H

#include <cube_positions.h>
#include <cube_rotations.h>
#include <smart_array.h>
#include <rubik.h>
#include <state_printer.h>

template< cube_size N >
class SubgroupAPI
{
  static constexpr size_t AllRot = CRotations<N>::AllRotIDs;
  
  size_t     m_size;
  BitMapID   m_priorRotIDs;

  const GroupID  * m_groupIDs;

public:
  SubgroupAPI();
  SubgroupAPI(  pArray< GroupID > );
  ~SubgroupAPI();

  void init(  pArray< GroupID > );

  size_t size() const
  {
    return m_size;
  }

  BitMapID priorRotIDs() const
  {
    return m_priorRotIDs;
  }

  bool priorMoving( const RotID rotID ) const
  {
    return m_priorRotIDs & ( 1ULL << rotID );
  }

  bool priorMoving( const CubeID prior, const RotID rotID ) const
  {
    return m_priorRotIDs & ( 1ULL << ( CRotations<N>::GetRotID( rotID, Simplex::Inverse( prior ) ) ) );
  }

  GroupID usePrior ( const CubeID prior, GroupID projected ) const
  {
    GroupID result = 0;
    for ( size_t i = 0; i < m_size; ++ i, projected /= 24 )
    {
      result += Simplex::Composition( projected % 24, prior ) * pow24( i );
    }
    return result;
  }

  GroupID lookUp( const GroupID gid, const RotID rid ) const
  {
    return m_groupIDs[ AllRot * gid + rid ];
  }
  
  GroupID lookUp( const GroupID gid, const RotID rid, const CubeID cid ) const
  {
    const RotID trans = CRotations<N>::GetRotID( rid, Simplex::Inverse( cid ) );
    return lookUp( gid, trans );
  }

  CubeID  getPrior( const Rubik<N> &, const CubeID trans ) const;
  GroupID getState( const Rubik<N> &, const CubeID trans ) const;


};

template< cube_size N >
SubgroupAPI<N>::SubgroupAPI()
 :  m_size( 0 )
 ,  m_priorRotIDs( 0 )
 ,  m_groupIDs       ( nullptr )
{
}

template< cube_size N >
SubgroupAPI<N>::SubgroupAPI( pArray< GroupID > sg )
{
  init( sg );
}
template< cube_size N >
void SubgroupAPI<N>::init(  pArray< GroupID > sg )
{
  m_size           = sg -> size;
  m_groupIDs       = sg -> data;
  m_priorRotIDs    = sg -> value;
}

template< cube_size N >
SubgroupAPI<N>::~SubgroupAPI()
{

}

#endif  //  ! SUBGROUP_CACHED__H
