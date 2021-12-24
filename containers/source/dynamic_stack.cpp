#include <dynamic_stack.h>


template< typename Type >
Stack<Type>::Stack()
{
}

template< typename Type >
Stack<Type>::Stack( const size_t size )
  : Array<Type>( size )
{
  reset();
}

template< typename Type >
void Stack<Type>::set( const size_t id )
{
  m_push = Array<Type>::at( id );
}

template< typename Type >
void Stack<Type>::push( const Type & next )
{
  *( m_push ++ ) = next;
}

template< typename Type >
void Stack<Type>::push( const Type * beg, const Type * end )
{
  while ( beg != end )
  {
    push( *( beg ++ ) );
  }
}

template< typename Type >
Type Stack<Type>::top() const
{
  return *( m_push - 1 );
}

template< typename Type >
Type Stack<Type>::pop()
{
  return *( -- m_push );
}

template< typename Type >
void Stack<Type>::pop( const size_t drop )
{
  m_push -= drop;
}

template< typename Type >
bool Stack<Type>::empty() const
{
  return Array<Type>::begin() == m_push;
}

template< typename Type >
size_t Stack<Type>::size() const
{
  return m_push - Array<Type>::begin();
}

template< typename Type >
Type * Stack<Type>::begin()
{
  return Array<Type>::begin();
}

template< typename Type >
Type * Stack<Type>::end()
{
  return m_push;
}

template< typename Type >
const Type * Stack<Type>::begin() const
{
  return Array<Type>::begin();
}

template< typename Type >
const Type * Stack<Type>::end() const
{
  return m_push;
}

template< typename Type >
void Stack<Type>::reset()
{
  m_push = Array<Type>::begin();
}

