#ifndef ___PROGRESS__H
#define ___PROGRESS__H

#include <progress_tree.h>
#include <guide_manager.h>
#include <guide_scheduler.h>

template < cube_size N >
class Progress: protected ProgressTree
              , protected GuideManager<N>
              , protected Scheduler<N>
{
  using Guide = typename GuideFactory<N>::Guide;

  Node * m_current;
  Node * m_root;
  size_t m_height;

  void findSolution();
  bool progress();
  bool solved() const;

  Sequence resolve() const;

public:
  Progress(): m_root( ProgressTree::root() ) {}
  bool logs = true;

  void toSolve( const Rubik<N> & );
  void addGuide( const ProgressTask, Pattern<N>, AcceptFunction af = Accept<N>::Normal );

  const Rubik<N> & solve( const int );

  bool setTree();
};

template< cube_size N >
void Progress<N>::toSolve( const Rubik<N> & cube )
{
  GuideManager<N>::setCube( cube );
}

template< cube_size N >
void Progress<N>::addGuide( const ProgressTask task, Pattern<N> pattern, AcceptFunction af )
{
  Guide guide = GuideFactory<N>::getGuide( pattern, af );
  GuideManager<N>::add( guide , task);
}

template< cube_size N >
const Rubik<N> & Progress<N>::solve( const int maxHeight )
{
  Sequence result;
  auto searchStopped = [ this, maxHeight ]()
  {
    const bool finished =  m_current -> solved() && GuideManager<N>::emptyPool( m_current );
    return finished || ( m_height > maxHeight );
  };

  // iteratively deepening algorithm
  m_current = ProgressTree::root();
  for ( m_height = 0; ! searchStopped(); ++ m_height )
  {
    findSolution();
  }
  return GuideManager<N>::getCube();
}

template< cube_size N >
bool Progress<N>::progress()
{
  while ( m_current -> hasChild() )
  {
    // descending to leaves
    do
    {
      m_current += GuideManager<N>::nextNode( m_current );
    }
    while ( m_current -> hasChild() );

    if ( solved() )
    {
      break;
    }

    // ascending while it's needed
    while ( ! m_current -> hasChild() && m_current != m_root )
    {
      -- m_current;
    }
  }
  return solved();
}

template< cube_size N >
void Progress<N>::findSolution()
{
  m_current = ProgressTree::root();
  if ( setTree() && progress() )
  {
    const Sequence result = resolve();
    GuideManager<N>::setCube( result );
    if ( logs )
    {
      CRotations<N>::Print( result );
      GuideManager<N>::showCube();
    }
    m_height = 0;
  }
}

template< cube_size N >
bool Progress<N>::solved() const
{
  return m_current -> solved();
}

template< cube_size N >
Sequence Progress<N>::resolve() const
{
  Sequence result;
  for ( const Node * P = ProgressTree::root(); P != m_current; ++ P )
  {
    result << P -> rotate;
  }
  return result;
}

template<cube_size N> bool Progress<N>::setTree()
{
  ProgressTree::set( m_height );
  return GuideManager<N>::setRoot( m_root );
}

#endif  //  ! ___PROGRESS__H
