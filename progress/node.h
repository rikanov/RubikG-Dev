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

  bool hasChild() const
  {
    // empty ( gradient == 0 ) and solved nodes ( gradient == 1 ) has no child --> stop searching
    return 0 < depth && 1 < gradient.data() && ! target.empty();
  }
};

#endif  //  ! ___TREE_NODE_H
