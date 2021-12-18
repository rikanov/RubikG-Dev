#ifndef ___SUB_TREE__H
#define ___SUB_TREE__H

#include <node.h>
#include <sequence.h>

class SubTree: public Array<Node>
{
  static constexpr size_t TreeHeight = 20;

public:
  SubTree()
    : Array<Node>( TreeHeight )
  {
  }

  void set( const size_t height )
  {
    for ( int d = 0; d <= height; ++ d )
    {
      Node & next = Array<Node>::get( d );
      next.reset();
      next.depth = height - d;
    }
  }

  Sequence getSolution()
  {
    // ToDo
    return Sequence();
  }
};

#endif  //  ! ___SUB_TREE__H
