#ifndef BITMAP__H
#define BITMAP__H

#include <base_types.h>

class BitMap
{
  BitMapID m_dataSet;
  uint8_t  m_nextID;

public:
  BitMap() = default;
  BitMap( const unsigned long ds )
  {
    set( ds );
  }
  void set( const unsigned long ds );
  bool next( uint8_t & nextID );
  bool operator >> ( uint8_t & nextID )
  {
    return next( nextID );
  }

  void print( const uint8_t length = 63, const uint8_t slice = 0 ) const;
  static void Print( const BitMapID, const uint8_t length = 63, const uint8_t slice = 0 );
};

#endif  //  ! BITMAP__H
