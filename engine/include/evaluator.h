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
  const Subgroup2<N> * m_subgroup;
  DistID     * m_nodeValue;
  BitMapID   * m_grade1;
  BitMapID   * m_grade2;
  BitMap32ID * m_aim1;
  BitMap32ID * m_aim2;

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
    return m_nodeValue[ gid ];
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
void Evaluator<N>::alloc()
{
  const size_t size = pow24( m_subgroup -> size() - 1 );
  m_nodeValue = new DistID    [ size ] {};
  m_grade1    = new BitMapID  [ size ] {};
  m_grade2    = new BitMapID  [ size ] {};
  m_aim1      = new BitMap32ID[ size ] {};
  m_aim2      = new BitMap32ID[ size ] {};
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
  BitMapID grad = dist == D ? m_grade1[ state ] : m_grade2[ state ];
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
  BitMap32ID aim = dist == D ? m_aim1[ state ] : m_aim2[ state ];
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
  clog ( "Qeueu size:", m_qeueu -> count() );
  for ( size_t i = 0; i < m_qeueu -> count(); ++ i )
  {
    const GroupID node = m_qeueu -> at( i );
    m_grade1[ node ] = 1;
    m_aim1  [ node ] = m_accept( m_subgroup -> priorPos() );
  }
  for ( size_t i = 0; i < m_qeueu -> count(); ++ i )
  {
    const GroupID parent = m_qeueu -> at( i );
    all_rotid ( rotID, N )
    {
      const GroupID child  = m_subgroup -> lookUp( parent, rotID );
      if ( m_qeueu -> used( child ) )
      {
        m_grade2 [ parent ] |= ( 1ULL << rotID );
        m_aim2   [ parent ] |= mergeAim( rotID, m_aim1[ child ] );
      }
    }
  }
}

template< cube_size N >
void Evaluator<N>::bindSolutionNodes( const GroupID parent, const RotID rotID )
{
  const GroupID  child = m_subgroup -> lookUp( parent, rotID );
  const RotID   rotInv = CRotations<N>::GetInvRotID( rotID );
  const BitMapID bitInvRotID = 1ULL << rotInv;
  if ( *m_qeueu << child )
  {
    m_nodeValue[ child ] = m_nodeValue[ parent ] + 1;
  }
  if ( 1 == m_nodeValue[ child ] - m_nodeValue[ parent ] )
  {
    m_grade1[ child ]  |= bitInvRotID;
    m_aim1  [ child ]  |= mergeAim( rotInv, m_aim1[ parent ] );
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
    all_rotid( rotID, N )
    {
      const GroupID neighbor = m_subgroup -> lookUp( gid, rotID );
      if ( m_nodeValue[ gid ] == m_nodeValue[ neighbor ] )
      {
        m_grade2[ gid ] |= ( 1ULL << rotID );
        m_aim2  [ gid ] |= mergeAim( rotID, m_aim1[ gid ] );
      }
    }
  }
}

template< cube_size N >
void Evaluator<N>::finishTargets()
{
  for ( GroupID node; *m_qeueu >> node; )
  {
    BitMap guide;
    guide.unit( CRotations<N>::AllRotIDs );
    guide.exclude( 1 | m_grade1[ node ] | m_grade2[ node ] );
    for ( RotID rotID = 0; guide >> rotID; )
    {
      const RotID   inv    = CRotations<N>::GetInvRotID( rotID );
      const GroupID former = m_subgroup -> lookUp( node, rotID );
      m_aim1[ former ] |= mergeAim( inv, m_aim1[ node ] );
      m_aim2[ former ] |= mergeAim( inv, m_aim2[ node ] );
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
