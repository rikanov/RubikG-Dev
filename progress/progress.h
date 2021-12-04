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
  Sequence result;

  int treeHeight = 0;
  while ( treeHeight <= maxHeight && ! ProgressTree<N>::treeHeight( treeHeight ) )
  {
    ++ treeHeight;
  }

  clog( Color::green, "tree height:", treeHeight,ProgressTree<N>::solved()  );
  if ( ProgressTree<N>::solved() )
  {
    return result;
  }

  return result;
}

#endif  //  ! ___PROGRESS__H
