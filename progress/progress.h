#ifndef ___PROGRESS__H
#define ___PROGRESS__H

#include <progress_tree.h>

template < cube_size N >
class Progress: protected ProgressTree<N>
{
  using Guide = typename GuideFactory<N>::Guide;

public:

  void toSolve( const Rubik<N> * cube )
  {
    ProgressTree<N>::m_cube = cube;
  }

  void addGuide( const ProgressTask, const size_t, const PosID *, AcceptFunction af = Accept<N>::Normal );
  Sequence startIDA( const int );
};

template< cube_size N >
void Progress<N>::addGuide( const ProgressTask task, const size_t size, const PosID * pattern, AcceptFunction af )
{
  Guide guide = GuideFactory<N>::getGuide( size, pattern, af );
  ProgressTree<N>::add( guide , task );
}

template< cube_size N >
Sequence Progress<N>::startIDA( const int maxHeight )
{
  for ( int height = 0; height <= maxHeight; ++ height )
  {
    ProgressTree<N>::setRoot();
    while ( ProgressTree<N>::progress() )
    {

    }
    if ( ProgressTree<N>::found() )
    {
      break;
    }
    ProgressTree<N>::increase(); clog( height, ProgressTree<N>::solved() );
  }

  CRotations<N>::Print( ProgressTree<N>::resolve() );

  return ProgressTree<N>::resolve();
}

#endif  //  ! ___PROGRESS__H
