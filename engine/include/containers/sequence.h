#ifndef ROTATION_SEQUENCE__H
#define ROTATION_SEQUENCE__H

#include < base_types.h >

class Sequence
{
  const RotID * m_rotations;
  size_t        m_size;
  
public:
  Sequence();
  Sequence( const RotID * rotations , const size_t size );
  Sequence( const Sequence & C );
  ~Sequence();
  
  const RotID * raw() const
  {
    return m_rotations;
  }
  
  size_t size() const
  {
    return m_size;
  }
  
  void set( const RotID * rotations, size_t size );
  bool     operator == ( const Sequence & S ) const;
  Sequence operator +  ( const Sequence & S ) const;
  void     operator += ( const Sequence & S ) ;
};


Sequence::Sequence()
  : m_sequence( nullptr )
  , m_size( 0 )
{
}

Sequence::Sequence( const RotID * rotations , const size_t size )
  : m_rotations( nullptr )
{
  set( rotations, size );
}

Sequence::Sequence( const Sequence & C )
  : m_rotations( nullptr )
{
  set( C.raw(), C.size() );
}

void Sequence::set( const RotID * rotations, size_t size )
{
  delete[] m_rotations;
  RotID * pool = new RotID[size];
  m_size = size;
  for( size_t id = 0; id < m_size; ++ id )
  {
    pool[id] = rotations[id];
  }
  m_rotations = pool;
}
  
bool Sequence::operator == ( const Sequence & S )
{
  if ( m_size != C.m_size )
    return false;

  for ( size_t id = 0; id < m_size; ++ id )
  {
    if ( m_rotations[id] != S.m_rotations[id] )
    {
      return false;
    }
  }
  return true;
}
  
Sequence Sequence::operator + ( const Sequence & S ) const
{
  Sequence result;
  RotID * pool = new RotID[ m_size + S.m_size ];
  size_t id = 0;

  for( ; id < m_size; ++ id )
  {
    pool[id] = m_rotations[id];
  }

  for( ; id < m_size + S.m_size; ++ id )
  {
    pool[id] = S.m_rotations[id];
  }

  result.m_rotations = pool;
  result.m_size = m_size + S.m_size;
  return result;
}
  
void Sequence::operator += ( const Sequence & S )
{
  RotID * pool = new RotID[ m_size + S.m_size ];
  size_t id = 0;

  for( ; id < m_size; ++ id )
  {
    pool[id] = m_rotations[id];
  }

  for( ; id < m_size + S.m_size; ++ id )
  {
    pool[id] = S.m_rotations[id];
  }

  delete[] m_rotations;
  m_rotations = pool;
  m_size += S.m_size;
}

Sequence::~Sequence()
{
  delete[] m_rotations;
  m_rotations = nullptr;
  m_size = 0;
}
  
#endif  // !  ROTATION_SEQUENCE__H
