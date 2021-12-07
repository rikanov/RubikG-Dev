#ifndef ___PROGRESS__H
#define ___PROGRESS__H

#include <progress_tree.h>

template < cube_size N >
class Progress: protected ProgressTree<N>
{
  using Guide = typename GuideFactory<N>::Guide;

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
  ProgressTree<N>::add( guide , task );
}

template< cube_size N >
Sequence Progress<N>::startIDA( const int maxHeight )
{

  for ( int height = 0; height <= maxHeight && ! ProgressTree<N>::findSolution(); ++ height )
  {
    ProgressTree<N>::increase();
  }

 const Sequence result = ProgressTree<N>::resolve();

 if ( logs )
 {
   CRotations<N>::Print( result );
 }

 return result;
}

#endif  //  ! ___PROGRESS__H
