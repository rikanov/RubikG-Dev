#ifndef QEUEU__H
#define QEUEU__H

#include <base_types.h>
#include <bool_array.h>

// an excluding FIFO object: any GroupID value can be pushed in only once a life time
class Qeueu: protected BoolArray
{
  size_t m_size;

  GroupID   * m_qeueudCubes ;
  GroupID   * m_qeuIn ;
  GroupID   * m_qeuOut;
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
    m_qeueudCubes = ( size == 0 ) ? nullptr : new GroupID [ size + 1 ];
    BoolArray::resize( size );
    m_qeuIn = m_qeuOut = m_qeueudCubes;    
  }
  
  void clean()
  {
    BoolArray::clean();
    m_qeuIn = m_qeuOut = m_qeueudCubes;    
  }
  
  void push_back( const GroupID& id )
  {
    BoolArray::set( id, true );
    *( m_qeuIn ++ ) = id;
  }

  GroupID at( const size_t id ) const
  {
    return *( m_qeuOut + id );
  }

  bool used( GroupID id ) const
  {
    return BoolArray::at( id );
  }
  
  bool operator << ( const GroupID& id )
  {
    if ( used( id ) == false )
    {
      BoolArray::set( id, true );
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

  size_t count() const
  {
    return m_qeuIn - m_qeuOut;
  }

  ~Qeueu()
  {
    delete[] m_qeueudCubes;
  }
};


#endif  //  ! QEUEU__H
