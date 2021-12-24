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
  Node * m_current;
  Node * m_root;

  bool progress();
  Sequence resolve() const;

  // iteratively deepening algorithm
  bool startIDA();

public:
  static constexpr BitMapID NoRestriction = ( 1ULL << CRotations<N>::AllRotIDs ) - 1;
  Progress(): m_numberOfSteps( 0 ), m_root( ProgressTree::root() ) {}
  bool consistency = true;

  void next( const size_t maxHeight = 10, const BitMapID restriction = NoRestriction );
  void next( const size_t maxHeight, const BitMap & );
  void reset();
  size_t solve( Rubik<N> & );
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
  consistency = true;
  GuideManager<N>::reset();
}


template< cube_size N >
size_t Progress<N>::solve( Rubik<N> & cube )
{
  m_cube = &cube;
  GuideManager<N>::m_cube = &cube;
  for ( size_t step = 0, set = 1; step < m_numberOfSteps; step += set )
  {
    if ( set )
    {
      ProgressTree::setStep( step );
      GuideManager<N>::setStep( step );
    }
    startIDA();
    if ( ! m_current -> solved() )
    {
      return step;
    }
    set = GuideManager<N>::emptyPool( m_current );
  }
  return 0;
}

template<cube_size N> bool Progress<N>::startIDA()
{
  bool solved = false;
  m_root = ProgressTree::root();
  m_current = ProgressTree::root();
  for ( size_t height = 0; ! solved && height <= ProgressTree::height(); ++ height )
  {
    ProgressTree::set( height );
    GuideManager<N>::setRoot( m_root );
    solved =  m_current -> solved() || progress();
  }
  if ( solved )
  {
    const Sequence result = resolve();
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

#endif  //  ! ___PROGRESS__H
