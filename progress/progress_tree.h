#ifndef ___PROGRESS_TREE__H
#define ___PROGRESS_TREE__H

#include <guide_handler.h>

template< cube_size N >
class ProgressTree: protected GuideHandler<N>
{
  using Guide = typename GuideFactory<N>::Guide;

  Array<Node> m_path;

  Node * m_current;

protected:

  static constexpr size_t TreeHeight = 11;

  const Rubik<N> * m_cube;

  ProgressTree();

  bool set( const size_t height );

  Node * root();
  const Node * root() const;

  void add( Guide, const ProgressTask );

  Sequence resolve( const Node * );
};

template< cube_size N >
ProgressTree<N>::ProgressTree()
  : m_path( TreeHeight )
{
  m_current = m_path.begin();
}

template< cube_size N >
bool ProgressTree<N>::set( const size_t height )
{
  for ( int d = 0; d <= height; ++ d )
  {
    m_path[d].depth = height - d;
  }
  return GuideHandler<N>::setRoot( m_path.begin(), m_cube );
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

template< cube_size N>
void ProgressTree<N>::add( ProgressTree::Guide guide, const ProgressTask task )
{
  GuideHandler<N>::add( guide, task );
}

#endif  //  ! ___PROGRESS_TREE__H
