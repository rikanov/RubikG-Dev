#ifndef ___PROGRESS_TREE__H
#define ___PROGRESS_TREE__H

#include <guide_handler.h>

template< cube_size N >
class ProgressTree: private GuideHandler<N>
{
  using Guide = typename GuideFactory<N>::Guide;

  Array<Node> m_path;

  Node * m_current;

protected:

  static constexpr size_t TreeHeight = 12;

  const Rubik<N> * m_cube;

  ProgressTree();

  bool treeHeight( int size );
  bool levelUp();
  bool levelDown( const RotID rotID );
  bool solved() const;

  void add( Guide, const ProgressTask );
};

template< cube_size N >
ProgressTree<N>::ProgressTree()
  : m_path( TreeHeight )
{
  m_current = m_path.begin();
}

template< cube_size N >
bool ProgressTree<N>::treeHeight( int size )
{
  auto P = m_path.begin();
  while ( size >= 0 )
  {
    ( P ++ ) -> depth = size --;
  }

  return GuideHandler<N>::setRoot( m_path.begin() );
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
bool ProgressTree<N>::levelDown( const RotID rotID )
{
  if ( m_current -> depth > 0 && GuideHandler<N>::nextNode( m_current, rotID ) )
  {
    ++ m_current;
    return true;
  }
  return false;
}

template< cube_size N >
bool ProgressTree<N>::solved() const
{
  return m_current -> gradient.contains( 0 );
}

template< cube_size N>
void ProgressTree<N>::add( ProgressTree::Guide guide, const ProgressTask task )
{
  m_current -> prior[ guide.index() ] = guide.currentPrior( m_cube );
  m_current -> state[ guide.index() ] = guide.currentState( m_cube );

  GuideHandler<N>::add( guide, task );
}

#endif  //  ! ___PROGRESS_TREE__H
