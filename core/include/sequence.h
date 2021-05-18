#ifndef ROTATION_SEQUENCE__H
#define ROTATION_SEQUENCE__H

#include <base_types.h>

class Sequence
{
  RotID * m_rotations;
  RotID * m_stackPointer;
  mutable const RotID * m_nextRotation;

public:
  Sequence();
  Sequence( const size_t );
  Sequence( const RotID * rotations, const size_t size );
  Sequence( const Sequence & C );
  ~Sequence();
  
  void set( const RotID * rotations, size_t size );
  void     operator += ( const Sequence & S ) ;
  bool     operator == ( const Sequence & S ) const;
  Sequence operator +  ( const Sequence & S ) const;

  Sequence & operator << ( const RotID rotID )
  {
    *( m_stackPointer++ ) = rotID;
    return *this;
  }

  Sequence & back()
  {
    --m_stackPointer;
    return *this;
  }

  RotID & last()
  {
    return *m_stackPointer;
  }

  void step()
  {
    ++ m_stackPointer;
  }

  const RotID * raw() const
  {
    return m_rotations;
  }

  size_t size() const
  {
    return m_stackPointer - m_rotations;
  }

  RotID start() const
  {
    m_nextRotation = m_rotations;
    return next();
  }

  RotID next() const
  {
    return *( m_nextRotation++ );
  }

};


#endif  // !  ROTATION_SEQUENCE__H
