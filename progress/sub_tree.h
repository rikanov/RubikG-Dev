#ifndef ___SUB_TREE__H
#define ___SUB_TREE__H

#include <node.h>

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
      Array<Node>::get( d ).depth = height - d;
    }
  }
};

#endif  //  ! ___SUB_TREE__H
