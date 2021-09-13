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
  const BitMapID    * m_gradient;
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
  
  BitMapID gradient( const GroupID gid ) const
  {
    return m_gradient[ gid ];
  }
};

template< cube_size N >
Evaluator<N>::Evaluator()
 : m_subgroup ( nullptr )
 , m_nodeValue( nullptr )
 , m_gradient ( nullptr )
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
  delete[] m_gradient;  
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
  DistID   * nodeValue = new DistID  [ pow24( m_subgroup -> size() - 1 ) ] {};
  BitMapID * gradient  = new BitMapID[ pow24( m_subgroup -> size() - 1 ) ] {};
  
  // 0 gradient: unsolvable
  // 1 gradient: solved state
  // initialize root nodes with zero RotID --> gradient = 1
  for ( size_t i = 0; i < m_qeueu.count(); ++ i )
    gradient[ m_qeueu.at( i ) ] = 1;

  GroupID parent;
  while ( m_qeueu >> parent )
  {
    all_rotid ( rotID )
    {
      const GroupID child = m_subgroup -> lookUp( parent, rotID, true );
      if ( m_qeueu << child )
      {
        nodeValue[ child ] = nodeValue[ parent ] + 1;
      }
      if ( nodeValue[ child ] == nodeValue[ parent ] + 1 )
      {
        gradient[ child ] |= ( 1ULL << CRotations<N>::GetInvRotID( rotID ) );
      }
    }
  }
  m_nodeValue = nodeValue;
  m_gradient  = gradient;
  m_qeueu.resize( 0 ); //  de-alloc queeu memory
}
#endif
