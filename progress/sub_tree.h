#ifndef ___SUB_TREE__H
#define ___SUB_TREE__H

#include <node.h>
#include <sequence.h>

class SubTree: public Array<Node>
{
  static constexpr size_t TreeHeight = 20;

  size_t m_maxHeight;

public:
  SubTree()
    : Array<Node>( TreeHeight )
    , m_maxHeight( 0 )
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

  void height( const size_t maxHeight )
  {
    m_maxHeight = maxHeight;
  }

  size_t height() const
  {
    return m_maxHeight;
  }

  Sequence getSolution( const Node * last )
  {
    Sequence result;
    for ( const Node * P = Array<Node>::begin(); P != last; ++ P )
    {
      result << P -> rotate;
    }
    return result;
  }
};

#endif  //  ! ___SUB_TREE__H
