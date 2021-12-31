#ifndef ___EVALUATOR_API__H
#define ___EVALUATOR_API__H

#include <gen_rotation_set.h>
#include <factories/evaluator_factory.h>

typedef uint8_t DistID;

template< cube_size N >
class GuideFactory<N>::EvaluatorAPI: public GuideFactory<N>::Evaluator
{

  CubeID m_rebase;
  CubeID m_inverseBase;

public:
  EvaluatorAPI();
  EvaluatorAPI( const Subgroup & subgroup, AcceptFunction af );

  void rebase( const CubeID rot )
  {
    m_rebase = rot;
    m_inverseBase = Simplex::Inverse( rot );
  }
  CubeID base() const
  {
    return m_rebase;
  }
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
  Pattern<N> pattern() const
  {
    return Pattern<N>::transpose( m_rebase );
  }
  bool stricterThan( const EvaluatorAPI & e ) const
  {
    return e.pattern() == pattern() && Accept<N>::Stricter( Evaluator::acceptance(), e.acceptance() );
  }
};

template< cube_size N >
GuideFactory<N>::EvaluatorAPI::EvaluatorAPI()
  : Evaluator()
  , m_rebase( 0 )
{
}

template< cube_size N >
GuideFactory<N>::EvaluatorAPI::EvaluatorAPI( const Subgroup & subgroup, AcceptFunction af )
  : Evaluator( subgroup, af )
  , m_rebase( 0 )
{

}

template< cube_size N > inline
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

template< cube_size N > inline
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
  return CubeSet::GetCubeSet( prior, aim, m_inverseBase );
}

#endif  //  ! ___EVALUATOR_API__H
