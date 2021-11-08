#ifndef GROUP_GENERATOR_FACTORY__H
#define GROUP_GENERATOR_FACTORY__H

#include <base_types.h>
#include <smart_array.h>
#include <patch_api.h>

template< cube_size N >
class FGenerator
{
  static constexpr size_t AllRot = CRotations<N>::AllRotIDs;
  
  void baseCube( GroupID * simplex, const size_t pos );
public:
  FGenerator() = default;
  pArray<GroupID> create( pArray<PosID> p );
};

template< cube_size N >
pArray<GroupID> FGenerator<N>::create( pArray<PosID> p )
{
  m_patch = p;
  const size_t size = AllRot * 24 * m_patch.size();
  GroupID * generator = new GroupID [ size ] {};
  for ( size_t pos = 0; pos < m_patch.size(); ++ pos )
  {
    baseCube( generator, pos );
  }
  return std::make_shared< Array<GroupID> >( size, generator );
}

template< cube_size N >
void FSGenerator<N>::baseCube( GroupID * generator, const size_t pos )
{
  all_cubeid ( cid )
  {
    RotID rid = 0;
    all_rot( axis, layer, turn, N )
    {
      ++ rid;
      CubeID res;
      if ( layer == CPositions<N>::GetLayer( m_patch.getPosID( pos ), cid, axis ) )
      {
        res = Simplex::Tilt( cid, axis, turn );
      }
      else
      {
        res = cid;
      }

      if ( m_patch.priorMoving( rid ) )
      {
        res = Simplex::Tilt( res, axis, 4 - turn );
      }
      generator[ ( 24 * pos + cid ) * AllRot + rid ] = res * pow24( pos );
    }
  }
}
#endif  //  ! GROUP_GENERATOR_FACTORY__H
