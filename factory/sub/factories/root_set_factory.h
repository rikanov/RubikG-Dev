#ifndef ___ROOT_SET_FACTORY__H
#define ___ROOT_SET_FACTORY__H

#include <dynamic_array.h>
#include <bitmap_set.h>
#include <APIs/pattern_api.h>

template< cube_size N >
class GuideFactory<N>::RootSet
{
  PatternAPI       m_pattern;
  AcceptFunction   m_accept;
  GroupID *        m_nextRoot;

  void init( AcceptFunction af );
  void addSolution( const CubeID invPrior, const size_t id, GroupID gid );
  void resolveAcceptance();
  void addRoot( const GroupID rootID );

protected:

  size_t         m_numberOfRoots;
  BitMap32ID     m_allowedPriors;
  Array<GroupID> m_setOfRoots;

  RootSet() = default;
  RootSet( const size_t size, const PosID * pos, AcceptFunction af = Accept<N>::Normal );
};


template< cube_size N >
GuideFactory<N>::RootSet::RootSet( const size_t size, const PosID * pos, AcceptFunction af )
  : m_pattern( size, pos )
  , m_numberOfRoots( 0 )
  , m_allowedPriors( 0 )
  , m_setOfRoots( 1 << ( 2 * size ) )
{
  init( af );
}

template< cube_size N >
void GuideFactory<N>::RootSet::init( AcceptFunction af )
{
  m_accept = af;
  m_allowedPriors = m_accept( m_pattern.getPriorPos() );

  // get all the root modes
  m_nextRoot  = m_setOfRoots.begin();
  resolveAcceptance();
}

template< cube_size N >
void GuideFactory<N>::RootSet::addSolution( const CubeID invPrior, const size_t id, GroupID gid )
{
  BitMap cubeSet( m_accept( m_pattern.getPosID( id ) ) );
  for ( CubeID next; cubeSet >> next; )
  {
    next = Simplex::Composition( next, invPrior );
    if ( 0 == id )
    {
      addRoot( gid + next );
    }
    else
    {
      addSolution( invPrior, id - 1, gid + next * pow24( id ) );
    }
  }
}

template< cube_size N >
void GuideFactory<N>::RootSet::resolveAcceptance()
{
  BitMap cubeSet( m_allowedPriors );
  for ( CubeID prior; cubeSet >> prior; )
  {
    addSolution( Simplex::Inverse( prior ), m_pattern.patternSize() - 2, 0 );
  }
}

template< cube_size N >
void GuideFactory<N>::RootSet::addRoot( const GroupID rootID )
{
  if (  m_pattern.valid( rootID ) )
  {
    ++ m_numberOfRoots;
    *( m_nextRoot ++ ) = rootID;
  }
}

#endif  //  ! ___ROOT_SET_FACTORY__H
