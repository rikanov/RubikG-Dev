#ifndef API_SUBGROUP__H
#define API_SUBGROUP__H

#include <cube_positions.h>
#include <factories/subgroup_factory.h>

template< cube_size N >
class GuideFactory<N>::SubgroupAPI: public GuideFactory<N>::Subgroup
{
  static constexpr size_t AllRot = CRotations<N>::AllRotIDs;

public:
  SubgroupAPI();
  SubgroupAPI( const Subgroup & sg );
  SubgroupAPI( Pattern<N> pattern );

  GroupID lookUp( const GroupID gid, const RotID rid ) const
  {
    return Subgroup::m_subgroupMap[ AllRot * gid + rid ];
  }
  
  GroupID lookUp( const CubeID prior, const GroupID state, RotID rotID ) const
  {
    return lookUp( state, Subgroup::m_transRotation[ 24 * rotID + prior ] );
  }
};


template< cube_size N >
GuideFactory<N>::SubgroupAPI::SubgroupAPI()
  : Subgroup()
{

}

template< cube_size N >
GuideFactory<N>::SubgroupAPI::SubgroupAPI( const Subgroup & sg )
  : Subgroup( sg )
{

}

template< cube_size N >
GuideFactory<N>::SubgroupAPI::SubgroupAPI( Pattern<N> pattern )
  : Subgroup( pattern )
{

}

#endif  //  ! API_SUBGROUP__H
