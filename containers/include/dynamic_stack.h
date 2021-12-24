#ifndef ___DYNAMIC_STACK__H
#define ___DYNAMIC_STACK__H

#include <dynamic_array.h>

template< typename Type >
class Stack: public Array<Type>
{

  Type * m_push;

protected:
  void set( const size_t id );

public:
  Stack();
  Stack( const size_t size );

  void push( const Type & next );
  void push( const Type * beg, const Type * end );

  Type top() const;
  Type pop();
  void pop( const size_t drop );

  bool empty() const;
  size_t size() const;

  Type * begin();
  Type * end();

  const Type * begin() const;
  const Type * end() const;

  void reset();
};

// include template source
#include "../source/dynamic_stack.cpp"

#endif  //  ! ___DYNAMIC_STACK__H
