#ifndef ROTATION_SEQUENCE__H
#define ROTATION_SEQUENCE__H

#include <base_types.h>
#include <dynamic_array.h>

class Sequence
{
public:
  
  static constexpr size_t StackSize = 2000;

private:
  Array<RotID>   m_rotations;
  RotID        * m_stackPointer;

  mutable const RotID * m_nextRotation;
  mutable const RotID * m_endSubsequence;

public:
  Sequence();
  Sequence( const size_t );
  Sequence( const RotID * rotations, const size_t size );
  
  void set( const RotID * rotations, size_t size );
  void     operator =  ( const Sequence & S ) ;
  void     operator += ( const Sequence & S ) ;
  bool     operator == ( const Sequence & S ) const;
  Sequence operator +  ( const Sequence & S ) const;

  void store( const RotID rotID );
  void back();
  Sequence & operator << ( const RotID rotID );
  const RotID * raw() const;
  size_t size() const;
  RotID start( const size_t size = 0 ) const;
  RotID next() const;
  void reset();
  void setState( const size_t );
  Sequence reverse() const;
  void save( const std::string & , const size_t size = 0 ) const;
  void load( const std::string & );
};


#endif  // !  ROTATION_SEQUENCE__H
