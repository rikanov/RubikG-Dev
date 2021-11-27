#ifndef ___RUBIK_TASK__H
#define ___RUBIK_TASK__H

#include <base_types.h>
#include <acceptance.h>
#include <signpost.h>

template < cube_size N >
struct Task
{
  size_t          patternSize;
  CubeID          patternTrans;
  const PosID   * pattern;
  AcceptFunction  accept;

  CubeID     startPrior;
  GroupID    startGroupID;

  Signpost * stack;
  DistID     level;
  DistID     depth;

  Task()
    : patternSize  ( 0 )
    , patternTrans ( 0 )
    , pattern( nullptr )
    , accept ( Accept<N>::Normal )

    , startPrior   ( 0 )
    , startGroupID ( 0 )

    , stack  ( new Signpost [ 200 ] )
    , level  ( 0 )
    , depth  ( 0 )
  {}

  Signpost * child()
  {
    return stack + level;
  }

  Signpost * parent()
  {
    return stack + level - 1;
  }

  bool levelUp();
  bool levelDown();

  ~Task()
  {
    delete[] stack;
  }
};

template< cube_size N >
bool Task<N>::levelUp()
{
  if ( 0 == level )
  {
    return false;
  }
  -- level;
  ++ depth;
  return true;
}

template< cube_size N >
bool Task<N>::levelDown()
{
  if ( 0 == depth || child() -> gradient.empty() )
  {
    return false;
  }
  ++ level;
  -- depth;
  return true;
}

#endif  //  ! ___RUBIK_TASK__H
