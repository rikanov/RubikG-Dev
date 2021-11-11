#ifndef FACTORY_PROGRESS__H
#define FACTORY_PROGRESS__H

#include <bitmap_set.h>
#include <factory_tree.h>


template< cube_size N >
class Progress
{
  static constexpr size_t MaxNumberOfWorkers = 2 * N * N;
  class Worker;
  class InitialState;
  class StateAPI;

  size_t     m_maxLevel;
  size_t     m_level;
  StateAPI * m_stateProgress;
  Worker   * m_workers;
  Worker   * m_nextWorker;

public:
  Progress()
    : m_maxLevel( 0 )
    , m_level( 0 )
    , m_stateProgress( nullptr )
    , m_workers( nullptr )
    {
      m_nextWorker = m_workers;
    };

  void setMaximumLevel( const size_t maxLevel )
  {
    m_maxLevel = maxLevel;
    m_stateProgress = new StateAPI [ maxLevel + 1 ];
  }

  void addWorker( const size_t size, const PosID * pos )
  {
   // typename Factory<N>::EvaluatorAPI a( size, pos, Accept<N>::Normal );
    // *( m_nextWorker ++ ) = Worker( m_maxLevel, m_level, a );
   // Worker( m_maxLevel, m_level, a );
  }

  ~Progress()
  {
    clog( "nullptr?", nullptr == m_workers );
    clog( "nullptr?", nullptr == m_stateProgress );
    delete[] m_stateProgress;
    delete[] m_workers;clog("deleted" );
  }
};

#include <progress_state.cpp>
#include <progress_worker.cpp>
#endif  //  ! FACTORY_PROGRESS__H
