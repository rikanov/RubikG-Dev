#ifndef ___GROUP_GENERATOR_API__H
#define ___GROUP_GENERATOR_API__H

#include <factories/generator_factory.h>

template< cube_size N >
class GuideFactory<N>::GroupGeneratorAPI: public GuideFactory<N>::GroupGenerator
{
  void addGenerator( GroupID * base, const GroupID * add );

protected:
  void generateBlock( const size_t pow, GroupID * cache );

public:
  GroupGeneratorAPI();
  GroupGeneratorAPI( const GroupGenerator & groupGenerator );
  GroupGeneratorAPI( Pattern<N> pattern );
  size_t groupSize() const;
};


template< cube_size N >
GuideFactory<N>::GroupGeneratorAPI::GroupGeneratorAPI()
  : GroupGenerator()
{
}

template< cube_size N >
GuideFactory<N>::GroupGeneratorAPI::GroupGeneratorAPI( Pattern<N> pattern )
  : GroupGenerator( pattern )
{
}

template< cube_size N >
size_t GuideFactory<N>::GroupGeneratorAPI::groupSize() const
{
  return GroupGenerator::m_groupSize;
}

template< cube_size N >
void GuideFactory<N>::GroupGeneratorAPI::generateBlock( const size_t pow, GroupID * cache )
{
  GroupID next = 0;
  all_cubeid( block )
  {
    const GroupID * add = this -> m_groupGenerators.get() + ( 24 * pow + block ) * CRotations<N>::AllRotIDs;
    for ( size_t line = 0; line < pow24( pow ); ++ line, ++ next )
    {
      addGenerator( cache + next * CRotations<N>::AllRotIDs, add );
    }
  }
}

template< cube_size N >
void GuideFactory<N>::GroupGeneratorAPI::addGenerator( GroupID * base, const GroupID * add )
{
  all_rotid( rotID, N )
  {
    *( base + rotID ) += *( add + rotID );
  }
}
#endif  //  ! ___GROUP_GENERATOR_API__H
