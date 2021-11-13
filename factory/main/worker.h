#ifndef PROGRESS_WORKER__TPP
#define PROGRESS_WORKER__TPP

#include <control_panel.h>
#include <state_api.h>
#include <factory_tree.h>

template< cube_size N >
class Factory<N>::Worker: protected ControlPanel
{
  static constexpr size_t MaxLevel = 200;
  CubeID   * m_priorProgress;
  GroupID  * m_stateProgress;

  bool restrict();
  void expand();

public:
  Worker()
    : m_priorProgress( nullptr )
    , m_stateProgress( nullptr )
  {}

  Worker( ControlPanel & cPanel, const EvaluatorAPI & evalAPI )
    : EvaluatorAPI ( evalAPI )
    , ControlPanel ( cPanel  )
    , m_priorProgress( new CubeID  [ MaxLevel ] )
    , m_stateProgress( new GroupID [ MaxLevel ] )
  {
  }
  bool finish() const;
};

template< cube_size N >
bool Factory<N>::Worker::restrict()
{
  const RotID   rotID   = ControlPanel::step();
  const CubeID  priorID = m_priorProgress[ ControlPanel::level() ];
  const GroupID stateID = m_stateProgress[ ControlPanel::level() ];
  const CubeID  rolled  = PatternAPI::movePrior( priorID, rotID );
  const GroupID nextID  = SubgroupAPI::lookUp( stateID, priorID );

  if ( ControlPanel::restrict( rolled, nextID ) )
  {
    m_priorProgress[ ControlPanel::level() + 1 ] = rolled;
    m_stateProgress[ ControlPanel::level() + 1 ] = nextID;
    return true;
  }
  return false;
}

template< cube_size N >
void Factory<N>::Worker::expand()
{
  const RotID   rotID   = ControlPanel::step();
  const CubeID  priorID = m_priorProgress[ ControlPanel::level() ];
  const GroupID stateID = m_stateProgress[ ControlPanel::level() ];
  const CubeID  rolled  = PatternAPI::movePrior( priorID, rotID );
  const GroupID nextID  = SubgroupAPI::lookUp( stateID, priorID );

  ControlPanel::expand( rolled, nextID );

  m_priorProgress[ ControlPanel::level() + 1 ] = rolled;
  m_stateProgress[ ControlPanel::level() + 1 ] = nextID;
}

template< cube_size N >
bool Factory<N>::Worker::finish() const
{
  const CubeID  priorID = m_priorProgress[ ControlPanel::level() ];
  const GroupID stateID = m_stateProgress[ ControlPanel::level() ];
  return ControlPanel::solved( priorID, stateID );
}
#endif  //  ! PROGRESS_WORKER__TPP
