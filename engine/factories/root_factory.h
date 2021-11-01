#ifndef ROOT_FACTORY__H
#define ROOT_FACTORY__H

#include <base_types.h>
#include <smart_array.h>
#include <acceptance.h>

template< cube_size N >
class FRoot
{
  pArray< PosID >  m_positions;
  AcceptFunction   m_accept;
  GroupID *        m_rootNodes;
  GroupID *        m_nextRoot;
  BitMap32ID       m_allowedRoots;
  size_t           m_valids;
  bool             m_count;

  bool valid( GroupID gid ) const;
  void addSolution( const CubeID invPrior, const size_t id, GroupID gid );
  void resolveAcceptance();
  void addRoot( const GroupID rootID );
public:
  FRoot() = default;
  pArray< GroupID > create( pArray< PosID > pos, AcceptFunction );
};

template< cube_size N >
pArray< GroupID > FRoot<N>::create( pArray< PosID > pos, AcceptFunction af )
{
  m_positions = pos;
  m_allowedRoots = m_accept( m_positions -> data[ m_positions -> size - 1 ] );
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
  BitMap cubeSet( m_accept( m_positions -> data[id] ) );
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
    addSolution( Simplex::Inverse( prior ), m_positions -> size - 2, 0 );
  }
}

template< cube_size N >
void FRoot<N>::addRoot( const GroupID rootID )
{
  const bool val = valid( rootID );
  if ( m_count )
  {
    ++ m_valids;
  }
  else
  {
    *( m_nextRoot ++ ) = rootID;
  }
}

template< cube_size N >
bool FRoot<N>::valid( GroupID gid ) const
{
  BoolArray positions( CPositions<N>::GetSize() );

  for ( size_t index = 0; index < m_positions -> size; ++ index, gid /= 24 )
  {
    const PosID next = CPositions<N>::GetPosID( m_positions -> data[ index ], gid % 24 );
    if ( positions( next ) )
      return false;
    positions.set( next, true );
  }
  return true;
}


#endif  //  ! ROOT_FACTORY__H
