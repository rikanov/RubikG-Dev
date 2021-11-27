#ifndef ___GROUP_GEERATOR_FACTORY__H
#define ___GROUP_GEERATOR_FACTORY__H

#include <factory_tree.h>

template< cube_size N >
class GuideFactory<N>::GroupGenerator: public GuideFactory<N>::PatternAPI
{
  static constexpr size_t AllRot = CRotations<N>::AllRotIDs;
  
  void baseCube( const size_t pos );
  void init();

protected:
  size_t         m_groupSize;
  Array<GroupID> m_groupGenerators;

  GroupGenerator();
  GroupGenerator( const size_t size, const PosID * pos );
};

template< cube_size N >
GuideFactory<N>::GroupGenerator::GroupGenerator()
  : PatternAPI()
  , m_groupSize( 0 )
{
}

template< cube_size N >
GuideFactory<N>::GroupGenerator::GroupGenerator( const size_t size, const PosID* pos )
  : PatternAPI( size, pos )
  , m_groupSize( pow24( PatternAPI::patternSize() - 1 ) )
{
  init();
}

template< cube_size N >
void GuideFactory<N>::GroupGenerator::init()
{
  const size_t size = AllRot * 24 * PatternAPI::patternSize();
  m_groupGenerators = Array<GroupID> ( size );
  for ( size_t pos = 0; pos < PatternAPI::patternSize(); ++ pos )
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
      if ( layer == CPositions<N>::GetLayer( this -> getPosID( pos ), cid, axis ) )
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
