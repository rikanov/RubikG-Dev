#ifndef SEEKER__H
#define SEEKER__H

#include <cube_set.h>
#include <qeueu.h>
#include <subgroup.h>
#include <bitmap_set.h>
#include <acceptance.h>

typedef uint8_t DistID;

template< cube_size N >
class Evaluator
{
  const Subgroup<N> * m_subgroup;
  const DistID      * m_nodeValue;
  const BitMapID    * m_grade1;
  const BitMapID    * m_grade2;
  const BitMap32ID  * m_aim1;
  const BitMap32ID  * m_aim2;

  AcceptFunction      m_accept;
  Qeueu             * m_qeueu;
  
  void dealloc();
  void addSolution ( const size_t, GroupID gid );
  
  BitMap32ID mergeSet( const RotID rotID, const BitMap32ID set ) const;

public:
  Evaluator ();
  ~Evaluator();
  
  void map    ( const Subgroup<N> * );
  void root   ( const GroupID );
  void accept ( AcceptFunction );
  void build  ();
  
  DistID distance( const GroupID gid ) const
  {
    return m_nodeValue[ gid ];
  }
  
  BitMapID grade1( const GroupID gid ) const
  {
    return m_grade1[ gid ];
  }

  BitMapID grade2( const GroupID gid ) const
  {
    return m_grade2[ gid ];
  }

  BitMap32ID aim1( const GroupID gid ) const
  {
    return m_aim1[gid];
  }

  BitMap32ID aim2( const GroupID gid ) const
  {
    return m_aim2[gid];
  }
};

template< cube_size N >
Evaluator<N>::Evaluator()
 : m_subgroup ( nullptr )
 , m_nodeValue( nullptr )
 , m_grade1   ( nullptr )
 , m_grade2   ( nullptr )
 , m_aim1     ( nullptr )
 , m_aim2     ( nullptr )
 , m_accept   ( Accept<N>::Normal )
 , m_qeueu    ( new Qeueu )
{
   
}
 
template< cube_size N >
Evaluator<N>::~Evaluator()
{
  dealloc();
  delete m_qeueu;
}

template< cube_size N >
void Evaluator<N>::dealloc()
{
  delete[] m_nodeValue;
  delete[] m_grade1;
  delete[] m_grade2;
  delete[] m_aim1;
  delete[] m_aim2;
}

template< cube_size N >
void Evaluator<N>::map( const Subgroup<N> * sg )
{
  m_qeueu -> resize( sg -> size() );
  m_subgroup = sg;
}

template< cube_size N >
void Evaluator<N>::root( const GroupID si )
{
  *m_qeueu << si;
}

template< cube_size N >
void Evaluator<N>::accept( AcceptFunction func )
{
  m_accept = func;
}

template< cube_size N >
void Evaluator<N>::addSolution( const size_t id, GroupID gid )
{
  BitMap cubeSet( m_accept( m_subgroup -> positions( id ) ) );
  for ( CubeID next; cubeSet >> next; )
  {
    if ( 0 == id )
    {
      *m_qeueu << Projection::LookUp( m_subgroup -> size(), gid + next );
    }
    else
    {
      addSolution( id - 1, gid + next * pow24( id ) );
    }
  }
}

template< cube_size N >
BitMap32ID Evaluator<N>::mergeSet( const RotID rotID, const BitMap32ID set ) const
{
  const BitMap32ID P = m_subgroup -> priorRotIDs();
  if ( P & ( 1ULL << rotID ) )
  {
    const RotID inv = CRotations<N>::GetInvRotID( rotID );
    return CubeSet::GetCubeSet( CRotations<N>::GetTilt( inv ), set );
  }
  return set;
}

template< cube_size N >
void Evaluator<N>::build()
{
  dealloc();
  addSolution( m_subgroup -> size() - 1, 0 );
  const BitMapID P = m_subgroup -> priorRotIDs();
  const size_t size = pow24( m_subgroup -> size() - 1 );
  DistID   * nodeValue = new DistID    [ size ] {};
  BitMapID * grade1    = new BitMapID  [ size ] {};
  BitMapID * grade2    = new BitMapID  [ size ] {};
  BitMap32ID * aim1    = new BitMap32ID[ size ] {};
  BitMap32ID * aim2    = new BitMap32ID[ size ] {};
  
  // first-grade gradient = 0 --> unsolvable
  // first_grade gradient = 1 --> solved state
  // initialize root nodes as solved with zero RotID --> first-grade gradient = 1
  clog ( "Qeueu size:", m_qeueu -> count() );
  for ( size_t i = 0; i < m_qeueu -> count(); ++ i )
  {
    const GroupID node = m_qeueu -> at( i );
    grade1[ node ] = 1;
    aim1  [ node ] = 1;
 
    all_rotid ( rotID, N ) 
    {
      if ( m_qeueu -> used( m_subgroup -> lookUp( node, rotID, true ) ) )
      {
        aim2  [ node ] |= ( P & ( 1ULL << rotID ) ) ? 1 << CRotations<N>::GetTilt( rotID ) : 1;
      }
    }
  }

  GroupID parent;
  while ( *m_qeueu >> parent )
  {
    all_rotid ( rotID, N )
    {
      const GroupID child = m_subgroup -> lookUp( parent, rotID, true );
      const BitMapID bitRotID = 1ULL << CRotations<N>::GetInvRotID( rotID );
      if ( *m_qeueu << child )
      {
        nodeValue[ child ] = nodeValue[ parent ] + 1;
      }
      if ( nodeValue[ child ] == nodeValue[ parent ] )
      {
        grade2[ child ] |= bitRotID;
        aim2  [ child ] |= mergeSet( rotID, aim1[ parent ] );
      }
      if ( nodeValue[ child ] == nodeValue[ parent ] + 1 )
      {
        grade1[ child ] |= bitRotID;
        grade2[ child ] |= bitRotID;
        aim1  [ child ] |= mergeSet( rotID, aim1[ parent ] );
        aim2  [ child ] |= mergeSet( rotID, aim2[ parent ] );
      }
    }
  }

  m_nodeValue = nodeValue;
  m_grade1    = grade1;
  m_grade2    = grade2,
  m_aim1      = aim1;
  m_aim2      = aim2;
  m_qeueu -> resize( 0 ); //  de-alloc queeu memory
}
#endif
