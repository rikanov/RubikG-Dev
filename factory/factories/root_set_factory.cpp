#ifndef FACTORY__ROOT__H
#define FACTORY__ROOT__H

#include <bool_array.h>
#include <bitmap_set.h>
#include <factory_tree.h>

template< cube_size N >
class Factory<N>::RootSet: public Factory<N>::ConnectionAPI
{
  AcceptFunction   m_accept;
  GroupID *        m_rootNodes;
  GroupID *        m_nextRoot;
  bool             m_count;

  void init( AcceptFunction af );
  void addSolution( const CubeID invPrior, const size_t id, GroupID gid );
  void resolveAcceptance();
  void addRoot( const GroupID rootID );

protected:
  size_t         m_numberOfRoots;
  BitMap32ID     m_allowedPriors;
  Array<GroupID> m_setOfRoots;

  RootSet() = default;
  RootSet( const PatchAPI & pa, AcceptFunction af = Accept<N>::Normal );
  RootSet( const size_t size, const PosID * pos, AcceptFunction af = Accept<N>::Normal );
};

template< cube_size N >
Factory<N>::RootSet::RootSet( const PatchAPI & pa, AcceptFunction af )
  : PatchAPI( pa )
{
  init( af );
}

template< cube_size N >
Factory<N>::RootSet::RootSet( const size_t size, const PosID * pos, AcceptFunction af )
  : ConnectionAPI( size, pos )
{
  init( af );
}

template< cube_size N >
void Factory<N>::RootSet::init( AcceptFunction af )
{
  m_accept = af;
  m_allowedPriors = m_accept( this -> getPriorPos() );

  // count root nodes to allocate
  m_numberOfRoots = 0;
  m_count  = true; // get number of valid states
  resolveAcceptance();
  m_setOfRoots = Array<GroupID>( m_numberOfRoots );

  // get all the root modes
  m_count  = false; // not just count but collect them
  m_rootNodes = m_setOfRoots.get();
  m_nextRoot  = m_rootNodes;
  resolveAcceptance();
}

template< cube_size N >
void Factory<N>::RootSet::addSolution( const CubeID invPrior, const size_t id, GroupID gid )
{
  BitMap cubeSet( m_accept( this -> getPosID( id ) ) );
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
void Factory<N>::RootSet::resolveAcceptance()
{
  BitMap cubeSet( m_allowedPriors );
  for ( CubeID prior; cubeSet >> prior; )
  {
    addSolution( Simplex::Inverse( prior ), PatchAPI::patchSize() - 2, 0 );
  }
}

template< cube_size N >
void Factory<N>::RootSet::addRoot( const GroupID rootID )
{
  if ( ! SubgroupAPI::valid( rootID ) )
    return;

  if ( m_count )
  {
    ++ m_numberOfRoots;
  }
  else
  {
    *( m_nextRoot ++ ) = rootID;
  }
}

#endif  //  ! FACTORY__ROOT__H
