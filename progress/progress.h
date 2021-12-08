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
  bool solved();

  Sequence resolve() const;

public:

  bool logs = true;

  void toSolve( const Rubik<N> * cube )
  {
    ProgressTree<N>::m_cube = cube;
  }

  void addGuide( const ProgressTask, const size_t, const PosID *, AcceptFunction af = Accept<N>::Normal );

  // iteratively deepening algorithm
  Sequence startIDA( const int );
};

template< cube_size N >
void Progress<N>::addGuide( const ProgressTask task, const size_t size, const PosID * pattern, AcceptFunction af )
{
  Guide guide = GuideFactory<N>::getGuide( size, pattern, af );
  ProgressTree<N>::add( guide , task);
}

template< cube_size N >
Sequence Progress<N>::startIDA( const int maxHeight )
{

  for ( m_height = 0; m_height <= maxHeight && ! findSolution(); ++ m_height );

  const Sequence result = resolve();
  if ( logs )
  {
    CRotations<N>::Print( result );
  }
  return result;
}

template< cube_size N >
bool Progress<N>::progress()
{
  Node * root = ProgressTree<N>::root();
  m_current   = root;
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
bool Progress<N>::solved()
{
  return m_current -> gradient.contains( 0 ) && ! m_current -> target.empty();
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
