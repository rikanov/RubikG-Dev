#ifndef ___GROUP_GEERATOR_FACTORY__H
#define ___GROUP_GEERATOR_FACTORY__H

#include <factory_tree.h>

template< cube_size N >
class GuideFactory<N>::GroupGenerator: protected Pattern<N>
{
  static constexpr size_t AllRot = CRotations<N>::AllRotIDs;
  
  void baseCube( const size_t pos );
  void init();

protected:
  size_t         m_groupSize;
  Array<GroupID> m_groupGenerators;

  GroupGenerator();
  GroupGenerator( Pattern<N> pattern );
};

template< cube_size N >
GuideFactory<N>::GroupGenerator::GroupGenerator()
  : m_groupSize( 0 )
{
}

template< cube_size N >
GuideFactory<N>::GroupGenerator::GroupGenerator( Pattern<N> pattern )
  : Pattern<N>( pattern )
  , m_groupSize( pow24( Pattern<N>::size() - 1 ) )
  , m_groupGenerators( AllRot * 24 * Pattern<N>::size() )
{
  init();
}

template< cube_size N >
void GuideFactory<N>::GroupGenerator::init()
{
  for ( size_t pos = 0; pos < Pattern<N>::size(); ++ pos )
  {
    baseCube( pos );
  }
}

template< cube_size N >
void GuideFactory<N>::GroupGenerator::baseCube( const size_t pos )
{
  all_cubeid ( cid )
  {
    RotID rid = 0;
    all_rot( axis, layer, turn, N )
    {
      ++ rid;
      CubeID res;
      if ( layer == CPositions<N>::GetLayer( Pattern<N>::get( pos ), cid, axis ) )
      {
        res = Simplex::Tilt( cid, axis, turn );
      }
      else
      {
        res = cid;
      }

      if ( this -> priorMoving( rid ) )
      {
        res = Simplex::Tilt( res, axis, 4 - turn );
      }
      m_groupGenerators[ ( 24 * pos + cid ) * AllRot + rid ] = res * pow24( pos );
    }
  }
}
#endif  //  ! ___GROUP_GEERATOR_FACTORY__H
