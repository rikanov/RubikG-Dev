#ifndef ___SUBGROUP_FACTORY__H
#define ___SUBGROUP_FACTORY__H

#include <APIs/generator_api.h>

template< cube_size N >
class GuideFactory<N>::Subgroup: public GuideFactory<N>::GroupGeneratorAPI
{
  static constexpr size_t AllRot = CRotations<N>::AllRotIDs;

  void copyLine( const GroupID *, GroupID * );
  void copyBlocks( const size_t );
  void createGroupCache();
protected:
  Array<GroupID> m_subgroupMap;

public:
  Subgroup();
  Subgroup( const GroupGeneratorAPI & groupGeneratorAPI );
  Subgroup( const size_t size, const PosID * pos );

};

template< cube_size N >
GuideFactory<N>::Subgroup::Subgroup()
  : GroupGeneratorAPI()
{
}

template< cube_size N >
GuideFactory<N>::Subgroup::Subgroup( const GroupGeneratorAPI & groupGeneratorAPI )
  : GroupGeneratorAPI( groupGeneratorAPI )
{
  createGroupCache();
}

template< cube_size N >
GuideFactory<N>::Subgroup::Subgroup( const size_t size, const PosID * pos )
  : GroupGeneratorAPI( size, pos )
{
  createGroupCache();
}

template< cube_size N >
void GuideFactory<N>::Subgroup::copyLine( const GroupID * from, GroupID * to )
{
  all_rotid( rotID, N )
  {
    *( to + rotID ) = *( from + rotID );
  }
}

template< cube_size N >
void GuideFactory<N>::Subgroup::copyBlocks( const size_t pow )
{
  GroupID next = 0;
  all_cubeid( block )
  {
    for ( size_t line = 0; line < pow24( pow ); ++ line, ++ next )
    {
      m_subgroupMap[ next * AllRot ] = next;
      copyLine( m_subgroupMap.get() + line * AllRot, m_subgroupMap.get() + next * AllRot );
    }
  }
}

template< cube_size N >
void GuideFactory<N>::Subgroup::createGroupCache()
{
  const size_t size = GroupGeneratorAPI::groupSize() * AllRot + 1;
  m_subgroupMap = Array<GroupID> ( size );
  for ( size_t pos = 0; pos < PatternAPI::patternSize() - 1; ++ pos )
  {
    copyBlocks ( pos );
    GroupGeneratorAPI::generateBlock( pos, m_subgroupMap.get() );
  }
}

#endif  //  ! ___SUBGROUP_FACTORY__H
