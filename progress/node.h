#ifndef ___TREE_NODE_H
#define ___TREE_NODE_H

#include <dynamic_array.h>
#include <bitmap_set.h>

struct Node
{
  static constexpr size_t MaxPatterns = 200;

  BitMap    gradient;
  BitMap    target;

  Array<CubeID>  prior;
  Array<GroupID> state;
  RotID          rotate;

  size_t depth;

  Node()
    : gradient( 0 )
    , target  ( 0 )
    , prior( MaxPatterns )
    , state( MaxPatterns )
    , rotate( 0 )
    , depth ( 0 )
  {
  }

  Node * end()
  {
    return this + depth;
  }

  bool leaf() const
  {
    return 0 == depth;
  }
};

#endif  //  ! ___TREE_NODE_H
