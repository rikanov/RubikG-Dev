#ifndef ___EVALUATOR_API__H
#define ___EVALUATOR_API__H

#include <gen_rotation_set.h>
#include <factories/evaluator_factory.h>

typedef uint8_t DistID;

template< cube_size N >
class GuideFactory<N>::EvaluatorAPI: public GuideFactory<N>::Evaluator
{

public:
  EvaluatorAPI();
  EvaluatorAPI( Pattern<N> pattern, AcceptFunction af = Accept<N>::Normal );

  BitMapID gradient( const CubeID prior, const GroupID state, const DistID D ) const;
  BitMap32ID target( const CubeID prior, const GroupID state, const DistID D ) const;

  bool accepted( const GroupID state ) const
  {
    return 0 == Evaluator::m_nodeChart[ state ].level;
  }
  DistID distance( const GroupID state ) const
  {
    return Evaluator::m_nodeChart[ state ].level;
  }
};

template< cube_size N >
GuideFactory<N>::EvaluatorAPI::EvaluatorAPI()
  : Evaluator()
{
}

template< cube_size N >
GuideFactory<N>::EvaluatorAPI::EvaluatorAPI( Pattern<N> pattern, AcceptFunction af )
  : Evaluator( pattern, af )
{
}

template< cube_size N >
BitMapID GuideFactory<N>::EvaluatorAPI::gradient( const CubeID prior, const GroupID state, const DistID dist ) const
{
  const DistID D = distance( state );
  if ( dist < D )
  {
    return 0;
  }
  if ( dist > D  )
  {
    return ( 1ULL << ( 9 * N + 1 ) ) - 2;
  }
  BitMapID grad = Evaluator::m_nodeChart[state].grade;
  GenerateRotationSet<N>::Transform( grad, prior );
  return grad;
}

template< cube_size N >
BitMap32ID GuideFactory<N>::EvaluatorAPI::target( const CubeID prior, const GroupID state, const DistID dist ) const
{
  const DistID D = distance( state );
  if ( dist < D )
  {
    return 0;
  }
  if ( dist > D  )
  {
    return ( 1 << 24 ) - 1;
  }
  const BitMap32ID aim = Evaluator::m_nodeChart[ state ].aim;
  return CubeSet::GetCubeSet( prior, aim );
}

#endif  //  ! ___EVALUATOR_API__H
