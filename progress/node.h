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

  bool hasChild() const
  {
    // empty ( gradient == 0 ) and solved nodes ( gradient == 1 ) has no child --> stop searching
    return 0 < depth && 1 < gradient.data() && ! target.empty();
  }

  bool alignedTo( const size_t index ) const
  {
    return target.contains( prior[ index ] );
  }

  bool restrict( const BitMap & grad, const BitMap & targ )
  {
    return gradient.restrict( grad ) && target.restrict( targ );
  }

  bool solved() const
  {
    return gradient.contains( 0 ) && ! target.empty();
  }
};

#endif  //  ! ___TREE_NODE_H
