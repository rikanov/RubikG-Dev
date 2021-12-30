#ifndef ___PROGRESS_TREE__H
#define ___PROGRESS_TREE__H

#include <dynamic_stack.h>
#include <sub_tree.h>

class ProgressTree: public Stack<SubTree>
{
  static constexpr size_t MaximumSteps = 20;

  SubTree * m_subTree;

protected:

  ProgressTree()
    : Stack<SubTree> ( MaximumSteps )
  {
    m_subTree = Stack<SubTree>::begin();
  }

  void setStep( const size_t step )
  {
    m_subTree = Stack<SubTree>::at( step );
  }

  void set( const size_t height )
  {
    m_subTree -> set( height );
  }

  size_t height() const
  {
    return m_subTree -> height();
  }

  void height( const size_t step, const size_t maxHeight )
  {
    Stack<SubTree>::at( step ) -> height( maxHeight );
  }

  Node * root()
  {
    return m_subTree -> begin();
  }

  const Node * root() const
  {
    return m_subTree -> begin();
  }

  Sequence getSolution( const Node * last ) const
  {
    return m_subTree -> getSolution( last );
  }

};

#endif  //  ! ___PROGRESS_TREE__H
