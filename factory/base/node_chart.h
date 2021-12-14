#ifndef NODE_CHART__H
#define NODE_CHART__H

struct NodeChart
{
  DistID      level   =  0;
  BitMapID    grade[2]= {};
  BitMap32ID  aim[2]  = {};
};

#endif  //  ! NODE_CHART__H