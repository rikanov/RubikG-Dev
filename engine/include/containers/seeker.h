#ifndef SEEKER__H
#define SEEKER__H

#include<rotation_set.h>
#include<qeueu.h>

typedef uint8_t DistID;

template< cube_size N >
class Seeker
{
  const Subgroup<N> * m_subgroup;
  const DistID      * m_nodeValue;
  const RotSetID    * m_gradient;
  Qeueu               m_qeueu;
  
  void dealloc();
  
public:
  Seeker ();
  ~Seeker();
  
  void map  ( const Subgroup<N> * );
  void root ( const SubgroupID    );
  void build();
  
};

template< cube_size N >
Seeker<N>::Seeker()
 : m_subgroup ( nullptr )
 , m_nodeValue( nullptr )
 , m_gradient ( nullptr )
{
   
}
 
template< cube_size N >
Seeker<N>::~Seeker()
{
  dealloc();
}

template< cube_size N >
void Seeker<N>::dealloc()
{
  delete[] m_nodeValue;
  delete[] m_gradient;  
}

template< cube_size N >
void Seeker<N>::map( const Subgroup<N> * sg )
{
  m_qeueu.resize( sg.size() );
  m_subgroup = sg;
}

template< cube_size N >
void Seeker<N>::root( const Subgroup si )
{
  m_qeueu << si;
}

template< cube_size N >
void Seeker<N>::build()
{
  dealloc();
  m_nodeValue = new DistID  [ pow24( m_subgroup -> size() - 1 ) ];
  m_gradient  = new RotSetID[ pow24( m_subgroup -> size() - 1 ) ];
  m_qeueu.resize( m_subgroup -> size() );
  
  SubgroupID parent;
  while ( m_qeueu >> parent )
  {
    m_subgroup.set( parent );
    all_rotid ( rotID )
    {
      const SubgroupID child = m_subgroup -> check( rotID, true );
      if ( m_qeueu << child )
      {
        m_nodeValue[ child ] = m_nodeValue[ parent ] + 1;
      }
      if ( m_nodeValue[ child ] == m_nodeValue[ parent ] + 1 )
      {
        m_gradient[ child ] |= ( 2 << CRotations<N>::Inverse( rotID ) );
      }
    }
  }
  m_qeueu.resize( 0 ); //  de-alloc queeu memory
}
#endif