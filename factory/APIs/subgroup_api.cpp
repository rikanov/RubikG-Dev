#ifndef API_SUBGROUP__H
#define API_SUBGROUP__H

#include <cube_positions.h>
#include <factory_tree.h>

template< cube_size N >
class Factory<N>::SubgroupAPI: public Factory<N>::Subgroup
{
  static constexpr size_t AllRot = CRotations<N>::AllRotIDs;
  
public:
  SubgroupAPI();
  SubgroupAPI( const Subgroup & sg );
  SubgroupAPI( const size_t size, const PosID * pos );

  GroupID lookUp( const GroupID gid, const RotID rid ) const
  {
    return Subgroup::m_subgroupMap[ AllRot * gid + rid ];
  }
  
  GroupID lookUp( const GroupID gid, const RotID rid, const CubeID cid ) const
  {
    const RotID trans = CRotations<N>::GetRotID( rid, Simplex::Inverse( cid ) );
    return lookUp( gid, trans );
  }

  bool valid( GroupID ) const;
};


template< cube_size N >
Factory<N>::SubgroupAPI::SubgroupAPI()
  : Subgroup()
{
}

template< cube_size N >
Factory<N>::SubgroupAPI::SubgroupAPI( const Subgroup & sg )
  : Subgroup( sg )
{
}

template< cube_size N >
Factory<N>::SubgroupAPI::SubgroupAPI( const size_t size, const PosID * pos )
  : Subgroup( size, pos )
{
}

template< cube_size N >
bool Factory<N>::SubgroupAPI::valid( GroupID gid ) const
{
  BoolArray positions( CPositions<N>::GetSize() );

  for ( size_t index = 0; index < this -> patchSize(); ++ index, gid /= 24 )
  {
    const PosID next = CPositions<N>::GetPosID( PatchAPI::getPosID( index ), gid % 24 );
    if ( positions( next ) )
      return false;
    positions.set( next, true );
  }
  return true;
}
#endif  //  ! API_SUBGROUP__H
