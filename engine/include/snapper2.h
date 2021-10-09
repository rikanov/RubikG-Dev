#ifndef SNAPPER2__H
#define SNAPPER2__H

#include <gen_rotation_set.h>
#include <evaluator.h>


struct Snapshots
{
  static constexpr size_t MaximumSubgroups = 40;

  BitMap       m_gradient;
  BitMap32ID   m_target;
  GroupID    * m_snapshot;
  GroupID    * m_endSnapshots;
  CubeID     * m_prior;

public:

  Snapshots()
   :  m_target   ( ( 1 << 24 ) - 1 )
   ,  m_snapshot ( new GroupID [ MaximumSubgroups ] )
   ,  m_prior    ( new CubeID  [ MaximumSubgroups ] )
  {
    m_endSnapshots = m_snapshot;
  }

  ~Snapshots()
  {
    delete[] m_snapshot;
    delete[] m_prior;
  }
};

template< cube_size N >
{
  static constexpr size_t MaximumIDAsteps = 12;

  Snapshots  * m_snapshot;
};

#endif  //  ! SNAPPER2__H
