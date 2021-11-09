#ifndef API_EVALUATOR__H
#define API_EVALUATOR__H

#include <gen_rotation_set.h>
#include <factory_tree.h>

typedef uint8_t DistID;

template< cube_size N >
class Factory<N>::EvaluatorAPI: public Factory<N>::Evaluator
{

public:
  EvaluatorAPI();
  EvaluatorAPI( const size_t size, const PosID * pos, AcceptFunction af = Accept<N>::Normal );

  BitMapID gradient( const CubeID prior, const GroupID state,  const DistID D ) const;
  BitMap32ID target( const CubeID prior, const GroupID state,  const DistID D ) const;

  DistID distance( const GroupID gid ) const
  {
    return this -> m_nodeChart.get()[ gid ].level;
  }
};

template< cube_size N >
Factory<N>::EvaluatorAPI::EvaluatorAPI()
  : Evaluator()
{
}

template< cube_size N >
Factory<N>::EvaluatorAPI::EvaluatorAPI( const size_t size, const PosID * pos, AcceptFunction af )
  : Evaluator( size, pos, af )
{
}

template< cube_size N >
BitMapID Factory<N>::EvaluatorAPI::gradient( const CubeID prior, const GroupID state,  const DistID dist ) const
{
  const DistID D = distance( state );
  if ( dist < D )
  {
    return 0;
  }
  if ( dist > D + 1 )
  {
    return ( 1ULL << ( 9 * N + 1 ) ) - 2;
  }
  BitMapID grad = this -> m_nodeChart.get()[state].grade[ ( dist > D ) ];
  GenerateRotationSet<N>::Transform( grad, prior );
  return grad;
}

template< cube_size N >
BitMap32ID Factory<N>::EvaluatorAPI::target( const CubeID prior, const GroupID state, const DistID dist ) const
{
  const DistID D = distance( state );
  if ( dist < D )
  {
    return 0;
  }
  if ( dist > D + 1 )
  {
    return ( 1 << 24 ) - 1;
  }
  const BitMap32ID aim = this -> m_nodeChart.get()[ state ].aim[ dist > D ];
  return CubeSet::GetCubeSet( prior, aim );
}

#endif  //  ! API_EVALUATOR__H
