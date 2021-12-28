#ifndef ___ROOT_SET_FACTORY__H
#define ___ROOT_SET_FACTORY__H

#include <bitmap_set.h>
#include <factory_tree.h>
#include <pattern.h>

template< cube_size N >
class GuideFactory<N>::RootSet
{
  AcceptFunction m_accept;
  GroupID *      m_nextRoot;
  Pattern<N>     m_pattern;
  void init( AcceptFunction af );
  void addSolution( const CubeID invPrior, const size_t id, GroupID gid );
  void resolveAcceptance();
  void addRoot( const GroupID rootID );

protected:

  size_t         m_numberOfRoots;
  BitMap32ID     m_allowedPriors;
  Array<GroupID> m_setOfRoots;

  RootSet() = default;
  RootSet( Pattern<N> pattern, AcceptFunction af = Accept<N>::Normal );

public:
  AcceptFunction acceptance() const;
};


template< cube_size N >
GuideFactory<N>::RootSet::RootSet( Pattern<N> pattern, AcceptFunction af )
  : m_pattern( pattern )
  , m_numberOfRoots( 0 )
  , m_allowedPriors( 0 )
  , m_setOfRoots( 1 << ( 2 * pattern.size() ) )
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
  BitMap cubeSet( m_accept( m_pattern.get( id ) ) );
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
    addSolution( Simplex::Inverse( prior ), m_pattern.size() - 2, 0 );
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

template< cube_size N >
AcceptFunction GuideFactory<N>::RootSet::acceptance() const
{
  return m_accept;
}

#endif  //  ! ___ROOT_SET_FACTORY__H
