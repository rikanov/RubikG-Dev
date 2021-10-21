#include <sequence.h>

#include <text_output.h>
#include <fstream>

Sequence::Sequence()
  : m_rotations( new RotID [ StackSize ] {} )
  , m_stackPointer( m_rotations )
{
}

Sequence::Sequence( const size_t size)
  : m_rotations( new RotID [size] {} )
  , m_stackPointer( m_rotations )
{
}

Sequence::Sequence(const RotID* rotations, const size_t size)
  : m_rotations( new RotID [ StackSize ] {} )
  , m_stackPointer( m_rotations )
{
  set( rotations, size );
}

void Sequence::store( const RotID rotID )
{
  *( m_stackPointer ++ ) = rotID;
}

Sequence & Sequence::operator << ( const RotID rotID )
{
  store( rotID );
  return *this;
}

const RotID * Sequence::raw() const
{
  return m_rotations;
}

size_t Sequence::steps() const
{
  return m_stackPointer - m_rotations;
}

RotID Sequence::start( const size_t size ) const
{
  m_nextRotation = m_rotations;
  m_endSubsequence = 0 == size ? m_stackPointer : m_rotations + size;
  return next();
}

RotID Sequence::next() const
{
  return ( m_nextRotation < m_endSubsequence ) ? *( m_nextRotation ++ ) : 0;
}

void Sequence::reset()
{
  m_stackPointer = m_rotations;
}

void Sequence::setState( const size_t size )
{
  m_stackPointer = m_rotations + size;
}

Sequence Sequence::reverse() const
{
  Sequence reversed;
  for ( const RotID * rot = m_stackPointer - 1; rot >= m_rotations; -- rot )
  {
    reversed << *rot;
  }
  return reversed;
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
  if ( steps() != S.steps() )
    return false;

  for ( size_t id = 0; id < steps(); ++ id )
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

void Sequence::save( const std::string & fname, const size_t size ) const
{
  std::ofstream writeSeq( fname,std::ios::trunc );
  if ( writeSeq.is_open() )
  {
    const size_t end = size == 0 ? steps() : size;
    for ( size_t step = 0; step < end; ++ step )
    {
      writeSeq << (int) m_rotations[ step ] << ' ';
    }
    writeSeq.close();
  }
  else
  {
    clog( Color::red, Color::flash, "ERR:", fname, "could not be open for writing.", Color::off );
  }
}

void Sequence::load( const std::string & fname )
{
  reset();
  std::ifstream readSeq( fname );
  m_stackPointer = m_rotations;
  if ( readSeq.is_open() )
  {
    for( int next = 0;  readSeq >> next; )
    {
      *( m_stackPointer ++ ) = next;
    }
    readSeq.close();
  }
  else
  {
    clog( Color::red, Color::flash, "ERR:", fname, "could not be open for writing.", Color::off );
  }
  
}

Sequence::~Sequence()
{
  delete[] m_rotations;
}
