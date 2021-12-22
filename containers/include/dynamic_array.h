#ifndef ___DYNAMIC_ARRAY__H
#define ___DYNAMIC_ARRAY__H

#include <memory>
#include <text_output.h>
#include <initializer_list>

template<class Type>
class Array
{
protected:
  size_t                  m_size;
  std::shared_ptr<Type[]> m_array;

public:
  Array();
  Array( const size_t size );

  void fill( const Type * beg, const Type * end );

  Type & get( const size_t & id );
  const Type & get( const size_t & id ) const;

  Type * at( const size_t & id );
  const Type * at( const size_t & id ) const;

  Type & operator[]( const size_t & id );
  const Type & operator[]( const size_t & id ) const;

  Type * operator() ( const size_t & id );
  const Type * operator() ( const size_t & id ) const;

  size_t size() const;

  Array & operator= ( const Array<Type> array );

  Type * begin();
  Type * end();

  const Type * begin() const;
  const Type * end() const;

  void init( const size_t size, const Type * data );
  void init( const std::initializer_list<Type> & list );
};

// include template source
#include "../source/dynamic_array.cpp"

#endif  //  ! ___DYNAMIC_ARRAY__H
