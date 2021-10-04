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
  void    set( const uint64_t ds );
  bool    next( uint8_t & nextID );
  uint8_t next();
  bool    empty() const
  {
    return m_dataSet == 0;
  }
  bool operator >> ( uint8_t & nextID )
  {
    return next( nextID );
  }

  void print_( const uint8_t length = 63, const uint8_t slice = 0 ) const;
  void print ( const uint8_t length = 63, const uint8_t slice = 0 ) const;

  static void Print_( const BitMapID,   const uint8_t length = 63, const uint8_t slice = 0 );
  static void Print_( const BitMap32ID, const uint8_t length = 32, const uint8_t slice = 0 );
  static void Print ( const BitMapID,   const uint8_t length = 63, const uint8_t slice = 0 );
  static void Print ( const BitMap32ID, const uint8_t length = 32, const uint8_t slice = 0 );
};

#endif  //  ! BITMAP__H
