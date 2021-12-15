#ifndef ___PROGRESS_TREE__H
#define ___PROGRESS_TREE__H

#include <guide_handler.h>

template< cube_size N >
class ProgressTree
{
  Array<Node> m_path;

protected:

  static constexpr size_t TreeHeight = 200;

  ProgressTree();

  bool setRoot();
  void set( const size_t height );

  Node * root();
  const Node * root() const;

  Sequence resolve( const Node * );
};

template< cube_size N >
ProgressTree<N>::ProgressTree()
  : m_path( TreeHeight )
{
}

template< cube_size N >
void ProgressTree<N>::set( const size_t height )
{
  for ( int d = 0; d <= height; ++ d )
  {
    m_path[d].depth = height - d;
  }
}

template< cube_size N >
Node * ProgressTree<N>::root()
{
  return m_path.begin();
}

template< cube_size N >
const Node * ProgressTree<N>::root() const
{
  return m_path.begin();
}

#endif  //  ! ___PROGRESS_TREE__H
