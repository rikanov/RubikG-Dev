#ifndef SNAPPER2__H
#define SNAPPER2__H

#include <base_types.h>
#include <gen_rotation_set.h>
#include <evaluator.h>

static constexpr size_t MaximumSubgroups = 40;


struct Snapshots
{
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
class Snapper2
{
  static constexpr size_t MaximumIDAsteps = 12;

  Snapshots    * m_snapshot;
  Subgroup2<N> * m_subgroup;
  Evaluator<N> * m_evaluate;

  Subgroup2<N> * m_endOfSubgroups;
  Evaluator<N> * m_endOfEvaluates;
  Snapshots    * m_deepestLevel;

public:

  Snapper2();
  ~Snapper2();

  void newTask( const PosID * ,const size_t , const CubeID orient = 0, AcceptFunction af = Accept<N>::Normal );
  void maximumTreeHeight( const DistID );
  bool progress();
};

template< cube_size N >
Snapper2<N>::Snapper2()
 :  m_snapshot( new Snapshots    [ MaximumIDAsteps  ] )
 ,  m_subgroup( new Subgroup2<N> [ MaximumSubgroups ] )
 ,  m_evaluate( new Evaluator<N> [ MaximumSubgroups ] )
{
  m_endOfSubgroups = m_subgroup;
  m_endOfEvaluates = m_evaluate;
  m_deepestLevel   = m_snapshot;
}

template< cube_size N >
void Snapper2<N>::newTask( const PosID* startPos, const size_t size, const CubeID orient, AcceptFunction acceptFunction )
{
  m_endOfSubgroups -> init   ( size, startPos, orient );
  m_endOfEvaluates -> map    ( m_endOfSubgroups );
  m_endOfEvaluates -> accept ( acceptFunction   );
  m_endOfEvaluates -> build();

  ++ m_endOfSubgroups;
  ++ m_endOfEvaluates;
}

template< cube_size N >
void Snapper2<N>::maximumTreeHeight( const DistID depth )
{
  m_deepestLevel = m_snapshot + depth;
}


template< cube_size N >
Snapper2<N>::~Snapper2()
{
  delete[] m_snapshot;
  delete[] m_subgroup;
  delete[] m_evaluate;
}


#endif  //  ! SNAPPER2__H
