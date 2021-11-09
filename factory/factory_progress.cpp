#ifndef FACTORY_PROGRESS__H
#define FACTORY_PROGRESS__H

#include <bitmap_set.h>
#include <factory_tree.h>


template< cube_size N >
class Factory<N>::Progress
{
  class Worker;
  class InitialState;
  class StateAPI;

  size_t     m_maxLevel;
  size_t     m_level;
  StateAPI * m_stateProgress;
  Worker   * m_workers;

public:
  Progress()
    : m_maxLevel( 0 )
    , m_level( 0 )
    , m_stateProgress( nullptr )
    , m_workers( nullptr )
    {
    };

  void setMaximumLevel( const size_t maxLevel )
  {
    m_maxLevel = maxLevel;
    m_stateProgress = new StateAPI [ maxLevel + 1 ];
  }

  virtual ~Progress()
  {
    delete[] m_stateProgress;
    delete[] m_workers;
  }
};

template< cube_size N >
class Factory<N>::Progress::InitialState
{
  void init();
protected:
  static Array<BitMapID> m_allowedRotations;
  InitialState();
};

template< cube_size N >
Factory<N>::Array<BitMapID> Factory<N>::Progress::InitialState::m_allowedRotations = Factory<N>::Array<BitMapID>( CRotations<N>::AllRotIDs );

template< cube_size N >
Factory<N>::Progress::InitialState::InitialState()
{
  if ( 0 == m_allowedRotations[0] )
  {
    init();
  }
}

template< cube_size N >
void Factory<N>::Progress::InitialState::init()
{
  constexpr size_t AllRot = CRotations<N>::AllRotIDs;
  constexpr BitMapID allRotations = ( 1ULL << AllRot ) - 1; // include the solved bit

  m_allowedRotations[0] = allRotations;

  RotID rotID = 0;
  BitMapID allow = allRotations;

  m_allowedRotations[ rotID ++ ] = allRotations;

  all_rot( axis, layer, turn, N )
  {
    if ( 1 == turn && 0 == layer )
      allow = allRotations;

    if ( 1 == turn )
      allow -= 7ULL << ( 3 * ( axis * N + layer ) + 1 );

    m_allowedRotations[ rotID ++ ] = allow;
  }
}

template< cube_size N >
class Factory<N>::Progress::StateAPI: public Factory<N>::Progress::InitialState
{

public:
  BitMap m_gradient;
  BitMap m_target;

  void initGradient( const RotID rotID )
  {
    m_gradient.set( InitialState::m_allowedRotations[ rotID ] );
  }
};

template< cube_size N >
class Factory<N>::Progress::Worker: public Factory<N>::EvaluatorAPI
{
  const size_t & m_maxLevel;
  const size_t & m_level;

  CubeID   * m_priorProgress;
  GroupID  * m_idProgress;

public:
  Worker() = default;
  Worker( const size_t & maxLevel, const size_t & level, const EvaluatorAPI & evalAPI )
    : EvaluatorAPI( evalAPI  )
    , m_maxLevel  ( maxLevel )
    , m_level     ( level    )
    , m_priorProgress( new CubeID  [ maxLevel + 1 ] )
    , m_idProgress   ( new GroupID [ maxLevel + 1 ] )
  {}

  bool makeStep( RotID rotID, BitMap & gradient, BitMap & target )
  {
    CubeID        priorID = m_priorProgress[ m_level ];
    const GroupID stateID = Subgroup::lookUp( m_idProgress[ m_level ] , rotID, priorID );
    const size_t  levelID =  m_maxLevel - m_level;

    PatchAPI::movePrior( priorID, rotID );

    m_idProgress[ m_level + 1 ] = stateID;

    if (  0 == gradient.restrict( EvaluatorAPI::gradient( priorID, stateID, levelID ) )
       || 0 ==   target.restrict( EvaluatorAPI::  target( priorID, stateID, levelID ) )
       )
    {
      return false;
    }
    m_priorProgress[ m_level + 1 ] = priorID;
    m_idProgress   [ m_level + 1 ] = stateID;
    return true;
  }
};

#endif  //  ! FACTORY_PROGRESS__H
