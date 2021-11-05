#ifndef ROOT_FACTORY__H
#define ROOT_FACTORY__H

#include <base_types.h>
#include <smart_array.h>
#include <bool_array.h>
#include <acceptance.h>
#include <patch_api.h>

template< cube_size N >
class FRoot
{
  PatchAPI<N>      m_patch;
  AcceptFunction   m_accept;
  GroupID *        m_rootNodes;
  GroupID *        m_nextRoot;
  BitMap32ID       m_allowedRoots;
  size_t           m_valids;
  bool             m_count;

  void addSolution( const CubeID invPrior, const size_t id, GroupID gid );
  void resolveAcceptance();
  void addRoot( const GroupID rootID );
public:
  FRoot() = default;
  pArray< GroupID > create( pArray< PosID > pos, AcceptFunction = Accept<N>::Normal );
};

template< cube_size N >
pArray< GroupID > FRoot<N>::create( pArray< PosID > pos, AcceptFunction af )
{
  m_patch.init( pos );
  m_accept = af;
  m_allowedRoots = m_accept( m_patch.getPriorPos() );
  m_valids = 0;
  m_count  = true; // get number of valid states
  resolveAcceptance();
  m_count  = false; // not just count but collect them
  m_rootNodes = new GroupID [ m_valids ];
  m_nextRoot  = m_rootNodes;
  resolveAcceptance();
  return std::make_shared< Array< GroupID > >( m_valids, m_rootNodes, m_allowedRoots );
}

template< cube_size N >
void FRoot<N>::addSolution( const CubeID invPrior, const size_t id, GroupID gid )
{
  BitMap cubeSet( m_accept( m_patch.getPosID( id ) ) );
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
void FRoot<N>::resolveAcceptance()
{
  BitMap cubeSet( m_allowedRoots );
  for ( CubeID prior; cubeSet >> prior; )
  {
    addSolution( Simplex::Inverse( prior ), m_patch.size() - 2, 0 );
  }
}

template< cube_size N >
void FRoot<N>::addRoot( const GroupID rootID )
{
  if ( ! m_patch.valid( rootID ) )
    return;
  if ( m_count )
  {
    ++ m_valids;
  }
  else
  {
    *( m_nextRoot ++ ) = rootID;
  }
}


#endif  //  ! ROOT_FACTORY__H
