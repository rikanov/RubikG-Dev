#ifndef EVALUATOR__H
#define EVALUATOR__H

#include <subgroup.h>

struct NodeChart
{
  DistID      value   =  0;
  BitMapID    grade[2]= {};
  BitMap32ID  aim[2]  = {};
};

template< cube_size N >
struct Evaluator
{
  const Subgroup<N>   subgroup;
  const NodeChart   * nodeChart;

  Evaluator( const Subgroup<N> & sub, const NodeChart * m )
    : subgroup  ( sub )
    , nodeChart ( m )
  {}
};

#endif  //  ! EVALUATOR__H
