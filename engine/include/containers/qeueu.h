#ifndef QEUEU__H
#define QEUEU__H


#include <initializer_list>
#include <base_types.h>
#include <bool_array.h>

// an excluding FIFO object: any GroupID value can be pushed in only once a life time
class Qeueu
{
  size_t m_size;

  GroupID * m_qeueudCubes ;
  GroupID * m_qeuIn ;
  GroupID * m_qeuOut;
  BoolArray m_used;
public:

  Qeueu()
  : m_size( 0 )
  , m_qeueudCubes( nullptr )
  {
  }

  void resize( const size_t size )
  {
    m_size = size;
    delete[] m_qeueudCubes;
    m_qeueudCubes = ( size == 0 ) ? nullptr : new GroupID [ pow24( size ) + 1 ];
    m_used.resize( pow24( m_size ) + 1 );
    m_qeuIn = m_qeuOut = m_qeueudCubes;    
  }
  
  size_t size() const
  {
    return m_qeuIn - m_qeuOut;
  }

  void clean()
  {
    m_used.clean();
    m_qeuIn = m_qeuOut = m_qeueudCubes;    
  }
  
  void push_back( const GroupID& id )
  {
    m_used.set( id, true );
    *( m_qeuIn ++ ) = id;
  }

  GroupID at( const size_t id ) const
  {
    return *( m_qeuOut + id );
  }

  bool operator << ( const GroupID& id )
  {
    if ( m_used( id ) == false )
    {
      m_used.set( id, true );
      *( m_qeuIn ++ ) = id;
      return true;
    }
    return false;
  }

  bool operator >> ( GroupID& id )
  {
      id = *( m_qeuOut ++ );
      return m_qeuOut <= m_qeuIn;
  }

  int count() const
  {
    return m_qeuIn - m_qeuOut;
  }

  ~Qeueu()
  {
    delete[] m_qeueudCubes;
  }
};


#endif  //  ! QEUEU__H
