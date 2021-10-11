#ifndef BITMAP__H
#define BITMAP__H

#include <base_types.h>

class BitMap
{
  BitMapID m_dataSet;
  uint8_t  m_nextID;

public:
  BitMap() = default;
  BitMap( const uint64_t ds )
  {
    set( ds );
  }
  
  void set( const uint64_t ds )
  {
    m_dataSet = ds;
    m_nextID  = 0;
  }
  
  void restrict( const uint64_t rs )
  {
    m_dataSet &= rs;  // set intersection
  }

  void expand( const uint64_t rs )
  {
    m_dataSet |= rs;  // set union
  }

  bool empty() const
  {
    return m_dataSet == 0;
  }
  
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
    m_dataSet &= UINT64_MAX - ( 1ULL << bit ) - 1;
  }

  bool contains( const size_t bit ) const
  {
    return m_dataSet & ( 1ULL << bit );
  }

  void print_( const uint8_t length = 63, const uint8_t slice = 0 ) const;
  void print ( const uint8_t length = 63, const uint8_t slice = 0 ) const;

  static void Print_( const BitMapID,   const uint8_t length = 63, const uint8_t slice = 0 );
  static void Print_( const BitMap32ID, const uint8_t length = 32, const uint8_t slice = 0 );
  static void Print ( const BitMapID,   const uint8_t length = 63, const uint8_t slice = 0 );
  static void Print ( const BitMap32ID, const uint8_t length = 32, const uint8_t slice = 0 );
};

#endif  //  ! BITMAP__H
