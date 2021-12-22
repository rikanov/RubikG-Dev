#ifndef ___ROTATION_SEQUENCE__H
#define ___ROTATION_SEQUENCE__H

#include <base_types.h>
#include <dynamic_stack.h>

class Sequence: public Stack<RotID>
{
public:
  
  static constexpr size_t StackSize = 2000;

public:
  Sequence();
  Sequence( const size_t );
  Sequence( const RotID * rotations, const size_t size );
  Sequence( const RotID * , const RotID * );
  
  void operator =  ( const Sequence & S ) ;
  void operator += ( const Sequence & S ) ;
  bool operator == ( const Sequence & S ) const;

  Sequence & operator << ( const RotID rotID );
  void save( const std::string & , const size_t size = 0 ) const;
  void load( const std::string & );
};


#endif  // !  ___ROTATION_SEQUENCE__H
