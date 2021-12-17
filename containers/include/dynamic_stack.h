#ifndef ___DYNAMIC_STACK__H
#define ___DYNAMIC_STACK__H

#include <dynamic_array.h>

template< typename Type >
class Stack: public Array<Type>
{

  Type * m_push;

protected:
  bool set( Type * start )
  {
    if( start <= m_push )
    {
      m_push = start;
      return true;
    }
    else
    {
      return false;
    }
  }

public:
  Stack( const size_t size )
    : Array<Type>( size )
    {
      reset();
    }

  void push( const Type & next )
  {
    *( m_push ++ ) = next;
  }

  void push( const Type * beg, const Type * end )
  {
    while ( beg != end )
    {
      push( *( beg ++ ) );
    }
  }

  Type top() const
  {
    return *( m_push - 1 );
  }

  Type pop()
  {
    return *( -- m_push );
  }

  void pop( const size_t drop )
  {
    m_push -= drop;
  }

  bool empty() const
  {
    return Array<Type>::begin() == m_push;
  }

  size_t size() const
  {
    return m_push - Array<Type>::begin();
  }

  Type * begin()
  {
    return Array<Type>::begin();
  }

  Type * end()
  {
    return Array<Type>::end();
  }

  const Type * begin() const
  {
    return Array<Type>::begin();
  }

  const Type * end() const
  {
    return Array<Type>::end();
  }

  void reset()
  {
    m_push = Array<Type>::begin();
  }
};
#endif  //  ! ___DYNAMIC_STACK__H
