#ifndef ___DYNAMIC_STACK__H
#define ___DYNAMIC_STACK__H

#include <dynamic_array.h>

template< typename Type >
class Stack: protected Array<Type>
{

  Type * m_push;

protected:
  Type * next()
  {
    return m_push;
  }

  void set( Type * start )
  {
    m_push = start;
  }

public:
  Stack( const size_t size )
    : Array<Type>( size )
    , m_push( Array<Type>::begin() )
    {}

  void push( const Type & next )
  {
    *( m_push ++ ) = next;
  }

  Type top() const
  {
    return *( m_push - 1 );
  }

  Type pop() const
  {
    return *( -- m_push );
  }

  bool empty() const
  {
    return Array<Type>::begin() == m_push;
  }

};
#endif  //  ! ___DYNAMIC_STACK__H
