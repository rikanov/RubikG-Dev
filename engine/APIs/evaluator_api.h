#ifndef EVALUATOR_API__H
#define EVALUATOR_API__H

#include <cube_set.h>
#include <bitmap_set.h>
#include <smart_array.h>
#include <node_chart.h>

typedef uint8_t DistID;

template< cube_size N >
class EvaluatorAPI
{
  pArray<NodeChart> m_nodeChart;
  
public:
  EvaluatorAPI ();

  void init( pArray< NodeChart > );
  
  BitMapID gradient( const CubeID prior, const GroupID state,  const DistID D ) const;
  BitMap32ID target( const CubeID prior, const GroupID state,  const DistID D ) const;

  DistID distance( const GroupID gid ) const
  {
    return m_nodeChart -> data[ gid ].value;
  }
};

template< cube_size N >
EvaluatorAPI<N>::EvaluatorAPI()
 : m_nodeChart( nullptr )
{
   
}
 
template< cube_size N >
void EvaluatorAPI<N>::init( pArray< NodeChart > sg )
{clog( "init", nullptr == sg -> data );
  m_nodeChart = sg;
}

template< cube_size N >
BitMapID EvaluatorAPI<N>::gradient( const CubeID prior, const GroupID state,  const DistID dist ) const
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
  BitMapID grad = m_nodeChart -> data[state].grade[ ( dist > D ) ];
  GenerateRotationSet<N>::Transform( grad, prior );
  return grad;
}

template< cube_size N >
BitMap32ID EvaluatorAPI<N>::target( const CubeID prior, const GroupID state, const DistID dist ) const
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
  const BitMap32ID aim = m_nodeChart -> data[ state ].aim[ dist > D ];
  return CubeSet::GetCubeSet( prior, aim );
}

#endif  //  ! EVALUATOR_API__H
