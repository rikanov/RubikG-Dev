#ifndef ___PROGRESS_TREE__H
#define ___PROGRESS_TREE__H

#include <guide_handler.h>

template< cube_size N >
class ProgressTree: private GuideHandler<N>
{
  using Guide = typename GuideFactory<N>::Guide;

  Array<Node> m_path;

  Node * m_current;

  bool setRoot();
  bool progress();
  bool solved();
  void checkLeaf();

protected:

  static constexpr size_t TreeHeight = 11;

  const Rubik<N> * m_cube;

  ProgressTree();

  void add( Guide, const ProgressTask );
  void increase();
  bool findSolution();
  Sequence resolve();
};

template< cube_size N >
ProgressTree<N>::ProgressTree()
  : m_path( TreeHeight )
{
  m_current = m_path.begin();
}

template< cube_size N >
bool ProgressTree<N>::setRoot()
{
  m_current   = m_path.begin();
  m_current = m_path.begin();
  return GuideHandler<N>::setRoot( m_path.begin(), m_cube );
}

template< cube_size N >
void ProgressTree<N>::increase()
{
  for ( Node * P = m_path.begin(), * end = P -> end(); P <= end; ++ P )
  {
    ++ ( P -> depth );
  }
}

template< cube_size N >
bool ProgressTree<N>::progress()
{
  while ( m_current -> hasChild() )
  {
    // descending to leaves
    while ( m_current -> hasChild() )
    {
      m_current += GuideHandler<N>::nextNode( m_current );
    }

    if ( solved() )
    {
      break;
    }

    // ascending while it's needed
    while ( ! m_current -> hasChild() && m_current != m_path.begin() )
    {
      -- m_current;
    }
  }
  return solved();
}

template< cube_size N >
bool ProgressTree<N>::findSolution()
{
  return setRoot() && progress();
}


template< cube_size N >
bool ProgressTree<N>::solved()
{
  return m_current -> gradient.contains( 0 ) && ! m_current -> target.empty();
}

template< cube_size N>
void ProgressTree<N>::add( ProgressTree::Guide guide, const ProgressTask task )
{
  GuideHandler<N>::add( guide, task );
}

template< cube_size N >
Sequence ProgressTree<N>::resolve()
{
  Sequence result;
  for ( auto P = m_path.begin(); P != m_current; ++ P )
  {
    result << P -> rotate;
    P -> rotate = 0;
    P -> depth  = 0;
  }

  m_current = m_path.begin();
  return result;
}


#endif  //  ! ___PROGRESS_TREE__H
