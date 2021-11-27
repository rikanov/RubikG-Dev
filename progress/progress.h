#ifndef ___PROGRESS__H
#define ___PROGRESS__H


#include <base_types.h>
#include <navigation.h>
#include <rubik.h>

template< cube_size N >
class Progress: protected Navigation<N>
              , public    Rubik<N>
{
  static constexpr DistID MaximumTreeHeight = 12;

  Task<N>         m_task;
  GuideFactory<N> m_factory;

  BitMapID * m_allowedGradient;

  void initGradients();

  bool levelUp();
  bool levelDown();
  bool guide( const RotID );
  bool progress();

  inline Signpost * child()
  {
    return m_task.actual();
  }

  inline Signpost * parent()
  {
    return m_task.actual() - 1;
  }

  inline DistID level() const
  {
    return m_task.level;
  }

  inline bool setNode( const RotID rotID )
  {
    return Navigation<N>::setNode();
  }

public:

  Progress();
  ~Progress();

  void addOptional ( const size_t size, const PosID * pattern, AcceptFunction af );
  void addScheduled( const size_t size, const PosID * pattern, AcceptFunction af );
  void startIDA();

};


template< cube_size N >
Progress<N>::Progress()
  : m_allowedGradient( new BitMapID[ CRotations<N>::AllRotIDs ] )
{
  initGradients();
}

template< cube_size N >
void Progress<N>::initGradients()
{
  constexpr BitMapID allRotations = ( 1ULL << CRotations<N>::AllRotIDs ) - 1; // include the solved bit

  RotID rotID    = 0;
  BitMapID allow = allRotations;

  m_allowedGradient[ rotID ++ ] = allRotations;

  all_rot( axis, layer, turn, N )
  {
    if ( 1 == turn && 0 == layer )
      allow = allRotations;

    if ( 1 == turn )
      allow -= 7ULL << ( 3 * ( axis * N + layer ) + 1 );

    m_allowedGradient[ rotID ++ ] = allow;
  }
}

template< cube_size N >
bool Progress<N>::guide( const RotID rotID )
{
  child() -> set( m_allowedGradient[ rotID ], parent() -> getTargetID() );
  return ( Navigation<N>::setNode( rotID ) );
}

template< cube_size N >
void Progress<N>::addOptional( const size_t size, const PosID * pattern, AcceptFunction accept )
{
  m_task.patternSize = size;
  m_task.pattern     = pattern;
  m_task.accept      = accept;

  Navigation<N>::addOptional( m_factory.getGuide( m_task ) );
}

template< cube_size N >
void Progress<N>::addScheduled( const size_t size, const PosID * pattern, AcceptFunction accept )
{
  m_task.patternSize = size;
  m_task.pattern     = pattern;
  m_task.accept      = accept;

  Navigation<N>::addScheduled( m_factory.getGuide( m_task ) );
}

template< cube_size N >
bool Progress<N>::progress()
{
  if ( child() -> gradient.contains( 0 ) )
  {
    return true;
  }

  auto emptyTree = [&]() { return level() == 0 && child() -> gradient.empty(); };

  bool result = false;
  for ( RotID nextBranch = 0; ! result && ! emptyTree(); )
  {
    if ( levelDown() && *parent() >> nextBranch && setNode( nextBranch ) )
    {
      result = child() -> gradient.contains( 0 );
    }
    else
    {
      levelUp();
    }
  }
  return result;

}

template<cube_size N> void Progress<N>::startIDA()
{
  for ( m_task.depth = 0; m_task.depth != MaximumTreeHeight; ++ m_task.depth )
  {
    if ( guide( 0 ) && progress() )
      return;
  }
}

template< cube_size N >
Progress<N>::~Progress()
{
  delete[] m_allowedGradient;
}

#endif  //  ! ___PROGRESS__H
