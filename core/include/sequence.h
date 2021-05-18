#ifndef ROTATION_SEQUENCE__H
#define ROTATION_SEQUENCE__H

#include <base_types.h>

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


#endif  // !  ROTATION_SEQUENCE__H
