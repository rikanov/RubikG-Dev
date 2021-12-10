#ifndef ___PROGRESS__H
#define ___PROGRESS__H

#include <progress_tree.h>

template < cube_size N >
class Progress: protected ProgressTree<N>
{
  using Guide = typename GuideFactory<N>::Guide;

  Node * m_current;
  size_t m_height;

  bool findSolution();
  bool progress();
  bool solved() const;

  Sequence resolve() const;

public:

  bool logs = true;

  void toSolve( const Rubik<N> & );
  void addGuide( const ProgressTask, const size_t, const PosID *, AcceptFunction af = Accept<N>::Normal );

  Sequence solve( const int );
};

template< cube_size N >
void Progress<N>::toSolve( const Rubik<N> & cube )
{
  ProgressTree<N>::setCube( cube );
}

template< cube_size N >
void Progress<N>::addGuide( const ProgressTask task, const size_t size, const PosID * pattern, AcceptFunction af )
{
  Guide guide = GuideFactory<N>::getGuide( size, pattern, af );
  ProgressTree<N>::add( guide , task);
}

template< cube_size N >
Sequence Progress<N>::solve( const int maxHeight )
{
  Sequence result;
  auto searchStopped = [ this, maxHeight ]()
  {
    const bool finished =  m_current -> solved() && GuideHandler<N>::emptyPool( m_current );
    return finished || ( m_height == maxHeight );
  };

  // iteratively deepening algorithm
  m_current = ProgressTree<N>::root();
  for ( m_height = 0; ! searchStopped(); ++ m_height )
  {
    if ( findSolution() )
    {
      result = resolve();
      ProgressTree<N>::setCube( result );
      if ( logs )
      {
        CRotations<N>::Print( result );
        ProgressTree<N>::showCube();
      }
    }
  }
  return Sequence();
}

template< cube_size N >
bool Progress<N>::progress()
{
  const Node * root = ProgressTree<N>::root();
  while ( m_current -> hasChild() )
  {
    // descending to leaves
    do
    {
      m_current += GuideHandler<N>::nextNode( m_current );
    }
    while ( m_current -> hasChild() );

    if ( solved() )
    {
      break;
    }

    // ascending while it's needed
    while ( ! m_current -> hasChild() && m_current != root )
    {
      -- m_current;
    }
  }
  return solved();
}

template< cube_size N >
bool Progress<N>::findSolution()
{
  return ProgressTree<N>::set( m_height ) && progress();
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
  for ( const Node * P = ProgressTree<N>::root(); P != m_current; ++ P )
  {
    result << P -> rotate;
  }
  return result;
}

#endif  //  ! ___PROGRESS__H
