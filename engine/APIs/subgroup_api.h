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
  
  size_t            m_size;
  pArray< GroupID > m_groupIDs;

public:
  SubgroupAPI();
  SubgroupAPI(  pArray< GroupID > );
  ~SubgroupAPI();

  void init(  pArray< GroupID > );

  size_t size() const
  {
    return m_size;
  }

  GroupID lookUp( const GroupID gid, const RotID rid ) const
  {
    return m_groupIDs -> data[ AllRot * gid + rid ];
  }
  
  GroupID lookUp( const GroupID gid, const RotID rid, const CubeID cid ) const
  {
    const RotID trans = CRotations<N>::GetRotID( rid, Simplex::Inverse( cid ) );
    return lookUp( gid, trans );
  }
};

template< cube_size N >
SubgroupAPI<N>::SubgroupAPI()
 :  m_size( 0 )
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
  m_groupIDs       = sg;
  m_size           = sg -> size;
}

template< cube_size N >
SubgroupAPI<N>::~SubgroupAPI()
{

}

#endif  //  ! SUBGROUP_CACHED__H
