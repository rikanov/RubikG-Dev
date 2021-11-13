#ifndef CONTROL_PANEL__H
#define CONTROL_PANEL__H

#include <APIs/evaluator_api.h>

template< cube_size N>
class Factory<N>::ControlPanel: public Factory<N>::EvaluatorAPI
{
private:
  const size_t * m_level;
  const size_t * m_depth;
  StateAPI     * m_nodes;
  const RotID  * m_step;

public:
  ControlPanel()
    : m_level( nullptr )
    , m_depth( nullptr )
    , m_nodes( nullptr )
    , m_step ( nullptr )
  {}
  ControlPanel( const size_t * refLevel, const size_t * refDepth, StateAPI * refState, const RotID * refStep )
    : m_level( refLevel )
    , m_depth( refDepth )
    , m_nodes( refState )
    , m_step ( refStep  )
  {}
  bool restrict( const CubeID priorID, const GroupID stateID );
  void extend  ( const CubeID priorID, const GroupID stateID );
  bool solved  ( const CubeID priorID, const GroupID stateID );

  size_t level() const
  {
    return *m_level;
  }

  RotID step() const
  {
    return *m_step;
  }
};

template< cube_size N >
bool Factory<N>::ControlPanel::restrict( const CubeID priorID, const GroupID stateID )
{
  const BitMapID gradient = EvaluatorAPI::gradient( priorID, stateID, *m_depth );
  const BitMap32ID target = EvaluatorAPI::  target( priorID, stateID, *m_depth );
  const StateAPI & node   = m_nodes[ *m_level + 1 ];

  if ( node.gradient.restrict( gradient ) && node.target.restrict( target ) )
  {
    return true;
  }
  else
  {
    return false;
  }
}

template< cube_size N >
void Factory<N>::ControlPanel::extend( const CubeID priorID, const GroupID stateID )
{
  const StateAPI & node   = m_nodes[ m_level + 1 ];

  node.gradient.restrict( EvaluatorAPI::gradient( priorID, stateID, m_depth ) );
  node.target.restrict  ( EvaluatorAPI::  target( priorID, stateID, m_depth ) );
}

template< cube_size N >
bool Factory<N>::ControlPanel::solved(const CubeID priorID, const GroupID stateID)
{
  const BitMapID accept = m_nodes[ m_level ].target.data();
  const bool     finish = EvaluatorAPI::gradient( priorID, stateID, 0 ).contains( 0 );
  const bool     placed = EvaluatorAPI::  target( priorID, stateID, 0 ).restrict( accept );
  return finish && placed;
}

#endif  //  ! CONTROL_PANEL__H
