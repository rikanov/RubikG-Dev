#include <sequence.h>



Sequence::Sequence()
  : m_rotations( new RotID [256] {} )
  , m_stackPointer( m_rotations )
{
}

Sequence::Sequence( const size_t size)
  : m_rotations( new RotID [size] {} )
  , m_stackPointer( m_rotations )
{
}

Sequence::Sequence(const RotID* rotations, const size_t size)
  : m_rotations( new RotID [256] {} )
  , m_stackPointer( m_rotations )
{
  set( rotations, size );
}

Sequence::Sequence( const Sequence & C )
  : m_rotations( new RotID [256] {} )
{
  set( C.raw(), C.size() );
}

void Sequence::set( const RotID * rotations, size_t size )
{
  m_stackPointer = m_rotations;
  for( size_t id = 0; id < size; ++ id )
  {
    *( m_stackPointer++ ) = rotations[id];
  }
}

bool Sequence::operator == ( const Sequence & S ) const
{
  if ( size() != S.size() )
    return false;

  for ( size_t id = 0; id < size(); ++ id )
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
  for( RotID n = start(); n; n = next() )
  {
    result << n;
  }
  for( RotID n = S.start(); n; n = S.next() )
  {
    result << n;
  }
  return result;
}

void Sequence::operator += ( const Sequence & S )
{
  for( RotID n = S.start(); n; n = S.next() )
  {
    *( m_stackPointer++ ) = n;
  }
}

Sequence::~Sequence()
{
  delete[] m_rotations;
}
