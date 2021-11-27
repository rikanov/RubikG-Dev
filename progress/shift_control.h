#ifndef ___SHIFT_CONTROL__H
#define ___SHIFT_CONTROL__H

#include <signpost.h>
#include <dynamic_array.h>
#include <worker.h>

template< cube_size N >
class ShiftControl
{
  using Worker = typename Factory<N>::Worker;

  Worker * m_scheduled;
  Worker * m_nextScheduled;

public:

  ShiftControl()
    : m_scheduled( new Worker [ 256 ] )
    , m_nextScheduled ( m_scheduled )
  {

  }

  ~ShiftControl()
  {
    delete[] m_scheduled;
  }

  void addScheduled( ProgressData & pData, const size_t size, const PosID * pos, AcceptFunction af )
  {
    *( m_nextScheduled ++ ) = Factory<N>::get();
  }

  bool setState() const
  {
    for ( auto p = m_scheduled; p != m_nextScheduled; ++ p )
    {
      p -> operate();
    }
  }
};

#endif  //  ! ___SHIFT_CONTROL__H
