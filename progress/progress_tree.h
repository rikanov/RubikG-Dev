#ifndef ___PROGRESS_TREE__H
#define ___PROGRESS_TREE__H

#include <guide_handler.h>

template< cube_size N >
class ProgressTree: private GuideHandler<N>
{
  using Guide = typename GuideFactory<N>::Guide;

  Array<Node> m_path;

  Node * m_current;
  Node * m_searchEnd;
protected:

  static constexpr size_t TreeHeight = 11;

  const Rubik<N> * m_cube;

  ProgressTree();

  bool setRoot();
  bool increase();
  bool levelUp();
  bool progress();
  bool solved() const;
  bool found() const;

  void add( Guide, const ProgressTask );

  Sequence resolve();
};

template< cube_size N >
ProgressTree<N>::ProgressTree()
  : m_path( TreeHeight )
{
  m_current   = m_path.begin();
  m_searchEnd = m_path.begin();
}

template< cube_size N >
bool ProgressTree<N>::setRoot()
{
  m_current   = m_path.begin();
  m_searchEnd = m_path.begin();
  return GuideHandler<N>::setRoot( m_path.begin() );
}

template< cube_size N >
bool ProgressTree<N>::increase()
{
  for ( Node * P = m_path.begin(), * end = P -> end(); P <= end; ++ P )
  {
    ++ ( P -> depth );
  }
  return true;
}

template< cube_size N >
bool ProgressTree<N>::levelUp()
{
  if ( m_current == m_path.begin() )
  {
    return false;
  }
  else
  {
    -- m_current;
    return true;
  }
}

template< cube_size N >
bool ProgressTree<N>::progress()
{
  while ( ! m_current -> gradient.empty() && 0 < m_current -> depth )
  {
    // step down if possible
    m_current += GuideHandler<N>::nextNode( m_current );
  }
  if ( solved() )
  {
    // end searching
    clog( Color::green, "end searching" );
    m_searchEnd = m_current;
    m_current   = m_path.begin();
  }

  return levelUp();
}

template< cube_size N >
bool ProgressTree<N>::solved() const
{
  return m_current -> gradient.contains( 0 );
}

template< cube_size N >
bool ProgressTree<N>::found() const
{
  return m_searchEnd != m_current || solved();
}

template< cube_size N>
void ProgressTree<N>::add( ProgressTree::Guide guide, const ProgressTask task )
{
  m_current -> prior[ guide.index() ] = guide.currentPrior( m_cube );
  m_current -> state[ guide.index() ] = guide.currentState( m_cube );

  GuideHandler<N>::add( guide, task );
}

template< cube_size N >
Sequence ProgressTree<N>::resolve()
{
  Sequence result;
  for ( auto P = m_path.begin(); P != m_searchEnd; ++ P )
  {
    result << P -> rotate;
    P -> rotate = 0;
    P -> depth  = 0;
  }

  m_current = m_path.begin();
  return result;
}


#endif  //  ! ___PROGRESS_TREE__H
