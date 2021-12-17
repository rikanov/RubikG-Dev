#ifndef ___PROGRESS__H
#define ___PROGRESS__H

#include <progress_tree.h>
#include <guide_manager.h>

template < cube_size N >
class Progress: protected ProgressTree
              , public GuideManager<N>
{
  using Guide = typename GuideFactory<N>::Guide;

  const int maxHeight = 10;

  size_t m_numberOfSteps;
  Node * m_current;
  Node * m_root;
  size_t m_height;

  void findSolution();
  bool progress();
  bool solved() const;

  Sequence resolve() const;

public:
  Progress(): m_numberOfSteps( 0 ), m_root( ProgressTree::root() ), m_height( 0 )  {}
  bool logs = true;

  void toSolve( const Rubik<N> & );
  void solve( const size_t );
  const Rubik<N> & start();

  bool setTree();
};

template< cube_size N >
void Progress<N>::toSolve( const Rubik<N> & cube )
{
  GuideManager<N>::m_cube = cube;
}

template< cube_size N >
void Progress<N>::solve( const size_t )
{
  ++ m_numberOfSteps;
  Scheduler<N>::nextSolution();
}


template< cube_size N >
const Rubik<N> & Progress<N>::start()
{
  Sequence result;
  auto searchStopped = [ this ]()
  {
    const bool finished =  m_current -> solved() && GuideManager<N>::emptyPool( m_current );
    return finished || ( m_height > this -> maxHeight );
  };

  m_current = ProgressTree::root();

  for ( size_t step = 0; step < m_numberOfSteps; ++ step )
  {
    GuideManager<N>::setStep( step );
    // iteratively deepening algorithm
    for ( m_height = 0; ! searchStopped(); ++ m_height )
    {
      findSolution();
    }
  }
  return GuideManager<N>::m_cube;
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
    GuideManager<N>::m_cube.rotate( result );
    if ( logs )
    {
      CRotations<N>::Print( result );
      GuideManager<N>::m_cube.print();
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
