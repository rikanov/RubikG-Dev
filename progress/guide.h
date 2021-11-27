#ifndef ___PROGRESS_GUIDE__H
#define ___PROGRESS_GUIDE__H

#include <signpost.h>
#include <APIs/evaluator_api.h>

template< cube_size N >
class GuideFactory<N>::Guide: public GuideFactory<N>::EvaluatorAPI
{
  struct GuideState
  {
    CubeID  prior;
    GroupID state;

  public:

    GuideState()
      : prior( 0 )
      , state( 0 )
    {

    }
  };

  Signpost   * m_stack;
  GuideState * m_states;

  const DistID * m_level;
  const DistID * m_depth;

  void nextState( const RotID rotID, GuideState * next  )
  {
    GuideState * actual = next - 1;
    next -> prior = PatternAPI::movePrior( actual -> prior, rotID );
    next -> state = SubgroupAPI::lookUp  ( actual -> state, rotID );
  }

public:

  Guide()
    : m_stack ( nullptr )
    , m_level ( nullptr )
    , m_depth ( nullptr )
  {

  }

  Guide( Task<N> & task )
    : EvaluatorAPI( task.patternSize, task.pattern, task.accept )
    , m_stack( task.stack )
    , m_level( & task.level )
    , m_depth( & task.depth )
  {
  }

  void restrict( const RotID rotID )
  {
    GuideState * next   = m_states + *m_level;
    GuideState * actual = next - 1;
    nextState( rotID, actual );

    const BitMapID   grad = EvaluatorAPI::gradient( next -> prior, next -> state, *m_depth );
    const BitMap32ID targ = EvaluatorAPI::target  ( next -> prior, next -> state, *m_depth );
    m_stack[ *m_level ].restrict( grad, targ );
  }

  void expand( const RotID rotID  )
  {
    GuideState * next   = m_states + *m_level;
    GuideState * actual = next - 1;
    nextState( rotID, actual );

    const BitMapID   grad = EvaluatorAPI::gradient( next -> prior, next -> state, *m_depth );
    const BitMap32ID targ = EvaluatorAPI::target  ( next -> prior, next -> state, *m_depth );
    m_stack[ *m_level ].expand( grad, targ );
  }

  Guide& operator= ( const Guide & ) noexcept { return *this; }
};
#endif  //  ! ___PROGRESS_GUIDE__H
