#ifndef ___PROGRESS_SCHEDULER__H
#define ___PROGRESS_SCHEDULER__H

#include <guide_container.h>

template< cube_size N >
class Scheduler
{
  GuideContainer<N> m_scheduled;
  GuideContainer<N> m_optional;

public:
  Scheduler() = default;
};
#endif  //  ! ___PROGRESS_SCHEDULER__H
