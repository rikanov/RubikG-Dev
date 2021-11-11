#ifndef PROGRESS_WORKER__TPP
#define PROGRESS_WORKER__TPP

template< cube_size N >
class Progress<N>::Worker
{
 // typename Factory<N>::EvaluatorAPI  m_evaluatorAPI;
  const size_t * m_maxLevel;
  const size_t * m_level;

  CubeID   * m_priorProgress;
  GroupID  * m_idProgress;

public:
  Worker()
    : m_maxLevel     ( nullptr )
    , m_level        ( nullptr )
    , m_priorProgress( nullptr )
    , m_idProgress   ( nullptr )
  {}

  Worker( const size_t & maxLevel, const size_t & level, const typename Factory<N>::EvaluatorAPI & evalAPI )
   // : m_evaluatorAPI (   evalAPI  )
    : m_maxLevel     ( & maxLevel )
    , m_level        ( & level    )
    , m_priorProgress( new CubeID  [ maxLevel + 1 ] )
    , m_idProgress   ( new GroupID [ maxLevel + 1 ] )
  {}

  Worker & operator= ( const Worker & w )
  {
    return *this;
  }

  bool makeStep( RotID rotID, BitMap & gradient, BitMap & target )
  {
    CubeID        priorID = m_priorProgress[ *m_level ];
    const GroupID stateID = Factory<N>::Subgroup::lookUp( m_idProgress[ *m_level ] , rotID, priorID );
    const size_t  levelID = m_maxLevel - m_level;

    Factory<N>::PatternAPI::movePrior( priorID, rotID );

    m_idProgress[ *m_level + 1 ] = stateID;

    if (  0 == gradient.restrict( Factory<N>::EvaluatorAPI::gradient( priorID, stateID, levelID ) )
       || 0 ==   target.restrict( Factory<N>::EvaluatorAPI::  target( priorID, stateID, levelID ) )
       )
    {
      return false;
    }
    m_priorProgress[ *m_level + 1 ] = priorID;
    m_idProgress   [ *m_level + 1 ] = stateID;
    return true;
  }
};

#endif  //  ! PROGRESS_WORKER__TPP