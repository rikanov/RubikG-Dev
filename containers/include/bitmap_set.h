#ifndef ___BITMAP__H
#define ___BITMAP__H

#include <initializer_list>
#include <base_types.h>

class BitMap
{
  using Init = std::initializer_list<uint64_t>;
  BitMapID m_dataSet;
  uint8_t  m_nextID;

public:
  BitMap(): m_dataSet( 0 ), m_nextID( 0 ) {};
  BitMap( const Init & );
  BitMap( const uint64_t ds )
  {
    set( ds );
  }
  
  operator BitMapID & ()
  {
    return m_dataSet;
  }

  operator BitMapID () const
  {
    return m_dataSet;
  }
  void set( const uint64_t ds )
  {
    m_dataSet = ds;
    m_nextID  = 0;
  }
  
  bool restrict( const BitMap & bm )
  {
    m_dataSet &= bm.m_dataSet;  // set intersection
    return 0 != m_dataSet;
  }

  bool restrict( const uint64_t rs )
  {
    m_dataSet &= rs; // set intersection
    return 0 != m_dataSet;
  }

  void expand( const BitMap & bm )
  {
    m_dataSet |= bm.m_dataSet;  // set union
  }

  void expand( const uint64_t rs )
  {
    m_dataSet |= rs;  // set union
  }

  void exclude( const uint64_t rs )
  {
    m_dataSet -= ( m_dataSet & rs );
  }

  void unit( const size_t size )
  {
    m_dataSet = ( 1ULL << size ) - 1;
  }

  bool empty() const
  {
    return m_dataSet == 0;
  }
  
  bool hasCommon( const BitMapID set ) const
  {
    return ( m_dataSet & set ) != 0;
  }

  uint8_t next();

  bool next( uint8_t & nextID );

  bool operator >> ( uint8_t & nextID )
  {
    return next( nextID );
  }

  void add( const size_t bit )
  {
    m_dataSet |= ( 1ULL << bit );
  }

  void remove( const size_t bit )
  {
    exclude( 1ULL << bit );
  }

  bool contains( const size_t bit ) const
  {
    return m_dataSet & ( 1ULL << bit );
  }

  const BitMapID & data() const
  {
    return m_dataSet;
  }

  void print_( const uint8_t length = 63, const uint8_t slice = 0 ) const;
  void print ( const uint8_t length = 63, const uint8_t slice = 0 ) const;

  static void Print_( const BitMapID,   const uint8_t length = 63, const uint8_t slice = 0 );
  static void Print_( const BitMap32ID, const uint8_t length = 32, const uint8_t slice = 0 );
  static void Print ( const BitMapID,   const uint8_t length = 63, const uint8_t slice = 0 );
  static void Print ( const BitMap32ID, const uint8_t length = 32, const uint8_t slice = 0 );
};

#endif  //  ! ___BITMAP__H
