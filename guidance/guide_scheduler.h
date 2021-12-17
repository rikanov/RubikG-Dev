#ifndef ___PROGRESS_SCHEDULER__H
#define ___PROGRESS_SCHEDULER__H

#include <guide_container.h>

template< cube_size N >
class Scheduler
{
  using Guide = typename GuideFactory<N>::Guide;

  GuideContainer<N> m_scheduled;
  GuideContainer<N> m_optional;

public:
  Scheduler() = default;

  void addGuide( const ProgressTask, const Guide & );
  void nextSolution();

  void expand( Stack<Guide> &, Stack<Guide> &, const size_t );
};

template< cube_size N >
void Scheduler<N>::addGuide( const ProgressTask ptask, const Guide & guide )
{
  if ( _Scheduled == ptask )
  {
    m_scheduled.add( guide );
  }
  else
  {
    m_optional.add( guide );
  }
}

template< cube_size N >
void Scheduler<N>::nextSolution()
{
  m_scheduled.nextSolution();
  m_optional.nextSolution();
}

template< cube_size N >
void Scheduler<N>::expand( Stack<Scheduler::Guide> & scheduled, Stack<Scheduler::Guide> & optional, const size_t step )
{
  m_scheduled.extend( scheduled, step );
  m_optional.extend( optional, step );
}

#endif  //  ! ___PROGRESS_SCHEDULER__H
