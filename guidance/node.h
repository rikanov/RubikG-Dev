#ifndef ___TREE_NODE_H
#define ___TREE_NODE_H

#include <dynamic_array.h>
#include <bitmap_set.h>

struct Node
{
  static constexpr size_t MaxPatterns = 200;

  DistID    asymmetry;
  BitMap    reverseSteps;
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
    return 0 < depth && ! gradient.empty() && ! target.empty();
  }

  bool restrict( const BitMap & grad, const BitMap & targ )
  {
    return gradient.restrict( grad ) && target.restrict( targ );
  }

  bool solved() const
  {
    return gradient.contains( 0 ) && ! target.empty();
  }

  void reset()
  {
    gradient = 0;
    target = 0;
  }

  void print( const size_t N ) const
  {
    gradient.print_( 9 * N + 1, 3 * N );
    target.print( 24, 4 );
  }

};

#endif  //  ! ___TREE_NODE_H
