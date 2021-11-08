#ifndef FACTORY_SUBGROUP__H
#define FACTORY_SUBGROUP__H

#include <factory_tree.h>

template< cube_size N >
class Factory<N>::Subgroup: public Factory<N>::GroupGeneratorAPI
{
  static constexpr size_t AllRot = CRotations<N>::AllRotIDs;

  void copyLine( const GroupID *, GroupID * );
  void copyBlocks( const size_t, GroupID * );
  void createGroupCache();
protected:
  cArray<GroupID> m_subgroupMap;

public:
  Subgroup( const GroupGeneratorAPI & groupGeneratorAPI );
  Subgroup( const size_t size, const PosID * pos );

};

template< cube_size N >
Factory<N>::Subgroup::Subgroup( const GroupGeneratorAPI & groupGeneratorAPI )
  : GroupGeneratorAPI( groupGeneratorAPI )
{
  createGroupCache();
}

template< cube_size N >
Factory<N>::Subgroup::Subgroup( const size_t size, const PosID * pos )
  : GroupGeneratorAPI( size, pos )
{
  createGroupCache();
}

template< cube_size N >
void Factory<N>::Subgroup::copyLine( const GroupID * from, GroupID * to )
{
  all_rotid( rotID, N )
  {
    *( to + rotID ) = *( from + rotID );
  }
}

template< cube_size N >
void Factory<N>::Subgroup::copyBlocks( const size_t pow, GroupID * cache )
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
void Factory<N>::Subgroup::createGroupCache()
{
  Array<GroupID> subgroup = MakeArray<GroupID>( new GroupID [ this -> groupSize() * AllRot + 1 ] {} );
  auto cache = subgroup.get();
  for ( size_t pos = 0; pos < this -> patchSize() - 1; ++ pos )
  {
    copyBlocks ( pos, cache );
    this -> generateBlock( pos, cache );
  }
  m_subgroupMap = subgroup;
}

#endif  //  ! FACTORY_SUBGROUP__H
