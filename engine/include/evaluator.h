#ifndef SEEKER__H
#define SEEKER__H

#include<qeueu.h>
#include<subgroup.h>

typedef uint8_t DistID;

template< cube_size N >
class Evaluator
{
  Subgroup<N> * m_subgroup;

  const DistID      * m_nodeValue;
  const BitMapID    * m_grade1;
  const BitMapID    * m_grade2;
  Qeueu               m_qeueu;
  
  void dealloc();
  
public:
  Evaluator ();
  ~Evaluator();
  
  void map  ( Subgroup<N> * );
  void root ( const GroupID );
  void build();
  
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
};

template< cube_size N >
Evaluator<N>::Evaluator()
 : m_subgroup ( nullptr )
 , m_nodeValue( nullptr )
 , m_grade1   ( nullptr )
 , m_grade2   ( nullptr )
{
   
}
 
template< cube_size N >
Evaluator<N>::~Evaluator()
{
  dealloc();
}

template< cube_size N >
void Evaluator<N>::dealloc()
{
  delete[] m_nodeValue;
  delete[] m_grade1;
  delete[] m_grade2;
}

template< cube_size N >
void Evaluator<N>::map( Subgroup<N> * sg )
{
  m_qeueu.resize( sg -> size() );
  m_subgroup = sg;
}

template< cube_size N >
void Evaluator<N>::root( const GroupID si )
{
  m_qeueu << si;
}

template< cube_size N >
void Evaluator<N>::build()
{
  dealloc();
  const size_t size = pow24( m_subgroup -> size() - 1 );
  DistID   * nodeValue = new DistID  [ size ] {};
  BitMapID * grade1    = new BitMapID[ size ] {};
  BitMapID * grade2    = new BitMapID[ size ] {};
  
  // first-grade gradient = 0 --> unsolvable
  // first_grade gradient = 1 --> solved state
  // initialize root nodes as solved with zero RotID --> first-grade gradient = 1
  for ( size_t i = 0; i < m_qeueu.count(); ++ i )
    grade1[ m_qeueu.at( i ) ] = 1;

  GroupID parent;
  while ( m_qeueu >> parent )
  {
    all_rotid ( rotID )
    {
      const GroupID child = m_subgroup -> lookUp( parent, rotID, true );
      const BitMapID bitRotID = 1ULL << CRotations<N>::GetInvRotID( rotID );
      if ( m_qeueu << child )
      {
        nodeValue[ child ] = nodeValue[ parent ] + 1;
      }
      if ( nodeValue[ child ] == nodeValue[ parent ] )
      {
        grade2[ child ] |= bitRotID;
      }
      if ( nodeValue[ child ] == nodeValue[ parent ] + 1 )
      {
        grade1[ child ] |= bitRotID;
        grade2[ child ] |= bitRotID;
      }
    }
  }
  m_nodeValue = nodeValue;
  m_grade1    = grade1;
  m_grade2    = grade2,
  m_qeueu.resize( 0 ); //  de-alloc queeu memory
}
#endif
