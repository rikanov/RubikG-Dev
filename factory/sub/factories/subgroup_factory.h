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
  void createTransRotations();

protected:
  Array<GroupID> m_subgroupMap;
  Array<RotID>   m_transRotation;

public:
  Subgroup();
  Subgroup( const GroupGeneratorAPI & groupGeneratorAPI );
  Subgroup( const Pattern<N> & pattern );

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
  createTransRotations();
}

template< cube_size N >
GuideFactory<N>::Subgroup::Subgroup( const Pattern<N> & pattern )
  : GroupGeneratorAPI( pattern )
  , m_subgroupMap( GroupGeneratorAPI::groupSize() * AllRot + 1 )
  , m_transRotation ( 24 * CRotations<N>::AllRotIDs )
{
  createGroupCache();
  createTransRotations();
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
      copyLine( m_subgroupMap.begin() + line * AllRot, m_subgroupMap.begin() + next * AllRot );
    }
  }
}

template< cube_size N >
void GuideFactory<N>::Subgroup::createGroupCache()
{
  for ( size_t pos = 0; pos < Pattern<N>::size() - 1; ++ pos )
  {
    copyBlocks ( pos );
    GroupGeneratorAPI::generateBlock( pos, m_subgroupMap.begin() );
  }
}

template< cube_size N >
void GuideFactory<N>::Subgroup::createTransRotations()
{
  auto P = m_transRotation.begin();
  all_rotid ( rotID, N )
  {
    all_cubeid ( prior )
    {
      // P[ 24 * rotID + prior ]
      P[ 24 * rotID + prior ] = CRotations<N>::GetRotID( rotID, Simplex::Inverse( prior ) );
    }
  }
}

#endif  //  ! ___SUBGROUP_FACTORY__H
