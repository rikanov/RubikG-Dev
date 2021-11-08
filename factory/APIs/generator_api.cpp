#ifndef API_GENERATOR__H
#define API_GENERATOR__H

#include <factory_tree.h>

template< cube_size N >
class Factory<N>::GroupGeneratorAPI: public Factory<N>::GroupGenerator
{
  void addGenerator( GroupID * base, const GroupID * add );

protected:
  void generateBlock( const size_t pow, GroupID * cache );

public:
  GroupGeneratorAPI( const GroupGenerator & groupGenerator );
  GroupGeneratorAPI( const size_t size, const PosID * pos );
  size_t groupSize() const;
};


template< cube_size N >
Factory<N>::GroupGeneratorAPI::GroupGeneratorAPI(const Factory::GroupGenerator& groupGenerator)
  : GroupGenerator( groupGenerator )
{
}

template< cube_size N >
Factory<N>::GroupGeneratorAPI::GroupGeneratorAPI( const size_t size, const PosID * pos )
  : GroupGenerator( size, pos )
{
}

template< cube_size N >
size_t Factory<N>::GroupGeneratorAPI::groupSize() const
{
  return this -> m_groupSize;
}

template< cube_size N >
void Factory<N>::GroupGeneratorAPI::generateBlock( const size_t pow, GroupID * cache )
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
void Factory<N>::GroupGeneratorAPI::addGenerator( GroupID * base, const GroupID * add )
{
  all_rotid( rotID, N )
  {
    *( base + rotID ) += *( add + rotID );
  }
}
#endif  //  ! API_GENERATOR__H
