#ifndef ___PROGRESS__H
#define ___PROGRESS__H

#include <progress_tree.h>
#include <guide_manager.h>

template < cube_size N >
class Progress: protected ProgressTree
              , public GuideManager<N>
{
  using Guide = typename GuideFactory<N>::Guide;

  Rubik<N> * m_cube;

  size_t m_numberOfSteps;
  size_t m_step;
  Node * m_current;
  Node * m_root;

  void reset();
  bool progress();

  // iteratively deepening algorithm
  bool startIDA();
public:
  static constexpr BitMapID NoRestriction = ( 1ULL << CRotations<N>::AllRotIDs ) - 1;
  Progress(): m_numberOfSteps( 0 ), m_root( ProgressTree::root() ) {}
  bool consistency = true;

  void next( const size_t maxHeight = 13, const BitMapID restriction = NoRestriction );
  void next( const size_t maxHeight, const BitMap & );
  void toSolve( Rubik<N> & );
  size_t start();
  size_t restart();
};

template< cube_size N >
void Progress<N>::next( const size_t maxHeight, const BitMap & bm )
{
  next( maxHeight, bm.data() );
}

template< cube_size N >
void Progress<N>::next( const size_t maxHeight, const BitMapID restriction )
{
  GuideManager<N>::restrict( restriction );
  ProgressTree::height( m_numberOfSteps ++, maxHeight );
  Scheduler<N>::nextSolution();
}
template< cube_size N >
void Progress<N>::reset()
{
  m_step = 0;
  consistency = true;
  GuideManager<N>::reset();
}

template< cube_size N >
void Progress<N>::toSolve( Rubik<N> & cube )
{
  reset();
  m_cube = &cube;
  GuideManager<N>::m_cube = &cube;
}

template< cube_size N >
size_t Progress<N>::start()
{
  for ( bool set = true; m_step < m_numberOfSteps; m_step += set )
  {clog( m_step, ':', m_numberOfSteps );
    if ( set )
    {
      ProgressTree::setStep( m_step );
      GuideManager<N>::setStep( m_step );
    }
    startIDA();
    if ( ! m_current -> solved() )
    {
      break;
    }
    set = GuideManager<N>::emptyPool( m_current );
  }
  return m_step;
}

template< cube_size N >
size_t Progress<N>::restart()
{
  reset();
  return start();
}

template<cube_size N> bool Progress<N>::startIDA()
{
  bool solved = false;
  m_root = ProgressTree::root();
  m_current = ProgressTree::root();
  for ( size_t height = 0; ! solved && height <= ProgressTree::height(); ++ height )
  {
    ProgressTree::set( height );
    GuideManager<N>::setRoot( m_root, height > 9 );
    solved =  m_current -> solved() || progress();
  }
  if ( solved )
  {
    const Sequence result = ProgressTree::getSolution( m_current );
    m_cube -> rotate( result );
    consistency &= CRotations<N>::Print( result );
    GuideManager<N>::m_cube -> print();
  }
  return solved;
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

    if ( m_current -> solved() )
    {
      break;
    }

    // ascending while it's needed
    while ( ! m_current -> hasChild() && m_current != m_root )
    {
      -- m_current;
    }
  }
  return m_current -> solved();
}

#endif  //  ! ___PROGRESS__H
