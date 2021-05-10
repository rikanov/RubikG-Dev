#ifndef QEUEU__H
#define QEUEU__H


#include <initializer_list>
#include <base_types.h>

constexpr size_t _pow24[] = { 1, 24, 576, 13824, 331776, 7962624 };
using SubSpace = const std::initializer_list <PosID>;

// an excluding FIFO object: any CacheID value can be pushed in only once a life time
class Qeueu
{
  const size_t m_size;

  CacheID * m_qeueudCubes ;
  CacheID * m_qeuIn ;
  CacheID * m_qeuOut;
  bool    * m_used;
public:

  Qeueu( const int& size )
  : m_size( size )
  , m_qeueudCubes( new CacheID [ _pow24[ size ] + 1 ] )
  {
    m_qeuIn = m_qeuOut = m_qeueudCubes;
    m_used = new bool [ _pow24[ m_size ] + 1 ] { false };
  }

  void push_back( const CacheID& id )
  {
    m_used[ id ] = true;
    *( m_qeuIn ++ ) = id;
  }

  CacheID at( const size_t id ) const
  {
    return *( m_qeuOut + id );
  }

  bool operator << ( const CacheID& id )
  {
    if ( m_used[ id ] == false )
    {
      m_used[ id ] = true;
      *( m_qeuIn ++ ) = id;
      return true;
    }
    return false;
  }

  bool operator >> ( CacheID& id )
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
    delete[] m_used;
  }
};


#endif  //  ! QEUEU__H
