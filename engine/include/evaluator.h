#ifndef EVALUATOR__H
#define EVALUATOR__H

#include <cube_set.h>
#include <qeueu.h>
#include <subgroup_cached.h>
#include <bitmap_set.h>
#include <acceptance.h>

typedef uint8_t DistID;

template< cube_size N >
class Evaluator
{
  struct Node
  {
    DistID      value   =  0;
    BitMapID    grade[2]= {};
    BitMap32ID  aim[2]  = {};
  };
  const Subgroup2<N> * m_subgroup;
  Node               * m_node;

  AcceptFunction   m_accept;
  Qeueu          * m_qeueu;
  
  void dealloc();
  void alloc();
  void resolveAcceptance();
  void createRoot();
  void bindSolutionNodes( const GroupID, const RotID );
  void buildSolutionGraph();
  void connectEqualNodes();
  void finishTargets();
  void addSolution ( const CubeID, const size_t, GroupID gid );
  
  BitMap32ID mergeAim( const RotID rotID, const BitMap32ID set ) const;

public:
  Evaluator ();
  ~Evaluator();
  
  void map    ( const Subgroup2<N> * );
  void addRoot   ( const GroupID );
  void accept ( AcceptFunction );
  void build  ();
  
  BitMapID gradient( const CubeID prior, const GroupID state,  const DistID D ) const;
  BitMap32ID target( const CubeID prior, const GroupID state,  const DistID D ) const;

  DistID distance( const GroupID gid ) const
  {
    return m_node[ gid ].value;
  }
};

template< cube_size N >
Evaluator<N>::Evaluator()
 : m_subgroup ( nullptr )
 , m_node     ( nullptr )
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
  delete[] m_node;
}

template< cube_size N >
void Evaluator<N>::alloc()
{
  m_node = new Node [ pow24( m_subgroup -> size() - 1 ) ] {};
}

template< cube_size N >
BitMapID Evaluator<N>::gradient( const CubeID prior, const GroupID state,  const DistID dist ) const
{
  const DistID D = distance( state );
  if ( dist < D )
  {
    return 0;
  }
  if ( dist > D + 1 )
  {
    return ( 1ULL << ( 9 * N + 1 ) ) - 2;
  }
  BitMapID grad = m_node[state].grade[ ( dist > D ) ];
  GenerateRotationSet<N>::Transform( grad, prior );
  return grad;
}

template< cube_size N >
BitMap32ID Evaluator<N>::target( const CubeID prior, const GroupID state, const DistID dist ) const
{
  const DistID D = distance( state );
  if ( dist < D )
  {
    return 0;
  }
  if ( dist > D + 1 )
  {
    return ( 1 << 24 ) - 1;
  }
  const BitMap32ID aim = m_node[ state ].aim[ dist > D ];
  return CubeSet::GetCubeSet( prior, aim );
}

template< cube_size N >
void Evaluator<N>::map( const Subgroup2<N> * sg )
{
  m_qeueu -> resize( sg -> size() );
  m_subgroup = sg;
}

template< cube_size N >
void Evaluator<N>::accept( AcceptFunction func )
{
  m_accept = func;
}

template< cube_size N >
BitMap32ID Evaluator<N>::mergeAim( const RotID rotID, const BitMap32ID set ) const
{
  if ( m_subgroup -> priorMoving( rotID ) )
  {
    return CubeSet::GetCubeSet( CRotations<N>::GetTilt( rotID ), set );
  }
  return set;
}

template< cube_size N >
void Evaluator<N>::addRoot( const GroupID rootID )
{
  if ( m_subgroup -> valid( rootID ) && *m_qeueu << rootID )
  {
   ; //  m_subgroup -> print( rootID, 0, true );
  }
}

template< cube_size N >
void Evaluator<N>::addSolution( const CubeID invPrior, const size_t id, GroupID gid )
{
  BitMap cubeSet( m_accept( m_subgroup -> positions( id ) ) );
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
void Evaluator<N>::resolveAcceptance()
{
  m_qeueu -> clean();
  BitMap cubeSet( m_accept( m_subgroup -> priorPos() ) );
  for ( CubeID prior; cubeSet >> prior; )
  {
    addSolution( Simplex::Inverse( prior ), m_subgroup -> size() - 2, 0 );
  }
}

template< cube_size N >
void Evaluator<N>::createRoot()
{
  // first-grade gradient = 0 --> unsolvable
  // first_grade gradient = 1 --> solved state
  // initialize root nodes as solved with zero RotID --> first-grade gradient = 1
  for ( size_t i = 0; i < m_qeueu -> count(); ++ i )
  {
    Node & node = m_node[ m_qeueu -> at( i ) ];
    node.grade[0] = 1;
    node.aim  [0] = m_accept( m_subgroup -> priorPos() );
  }
}

template< cube_size N >
void Evaluator<N>::bindSolutionNodes( const GroupID parent, const RotID rotID )
{
  const GroupID  child = m_subgroup -> lookUp( parent, rotID );
  Node       & cNode = m_node[ child ];
  const Node & pNode = m_node[ parent ];
  if ( *m_qeueu << child )
  {
    cNode.value = pNode.value + 1;
  }
  if ( cNode.value == pNode.value + 1 )
  {
    const RotID rotInv = CRotations<N>::GetInvRotID( rotID );
    cNode.grade[0] |= 1ULL << rotInv;
    cNode.aim[0]   |= mergeAim( rotInv, pNode.aim[0] );
  }
}

template< cube_size N >
void Evaluator<N>::buildSolutionGraph()
{
  GroupID parent;
  while ( *m_qeueu >> parent )
  {
    all_rotid ( rotID, N )
    {
      bindSolutionNodes( parent, rotID );
    }
  }
}

template< cube_size N >
void Evaluator<N>::connectEqualNodes()
{
  const size_t size = pow24( m_subgroup -> size() - 1 );
  for ( GroupID gid = 0; gid < size; ++ gid )
  {
    if ( ! m_subgroup -> valid( gid ) )
    {
      continue;
    }
    Node & node = m_node[gid];
    all_rotid( rotID, N )
    {
      const GroupID neighbor = m_subgroup -> lookUp( gid, rotID );
      if ( node.value == m_node[ neighbor ].value )
      {
        node.grade[1] |= ( 1ULL << rotID );
        node.aim  [1] |= mergeAim( rotID, node.aim[0] );
      }
    }
    node.grade[1] |= node.grade[0];
  }
}

template< cube_size N >
void Evaluator<N>::finishTargets()
{
  for ( GroupID node; *m_qeueu >> node; )
  {
    BitMap guide;
    guide.unit( CRotations<N>::AllRotIDs );
    guide.exclude( 1 | m_node[node].grade[0] | m_node[node].grade[1] );
    for ( RotID rotID = 0; guide >> rotID; )
    {
      const RotID   inv     = CRotations<N>::GetInvRotID( rotID );
      const GroupID former  = m_subgroup -> lookUp( node, rotID );
      m_node[former].aim[0] |= mergeAim( inv, m_node[node].aim[0] );
      m_node[former].aim[1] |= mergeAim( inv, m_node[node].aim[1] );
      *m_qeueu << former;
    }
  }
}

template< cube_size N >
void Evaluator<N>::build()
{
  dealloc();
  alloc();
  resolveAcceptance();
  createRoot();
  buildSolutionGraph();
  connectEqualNodes();
  resolveAcceptance();
  finishTargets();
  m_qeueu -> resize( 0 ); //  de-alloc queeu memory
}
#endif
