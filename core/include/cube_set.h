#ifndef FINAL_STATES_CACHE__H
#define FINAL_STATES_CACHE__H

#include<base_types.h>

// 24-bit wide data to represent sets of CubeIDs as possible final states of the prior cube
typedef uint32_t CubeSetID;

class CubeSet
{
public:
  enum TransMode
  {
    trNormal,
    trLow,
    trHigh
  };

private:
  static CubeSet * Singleton;
  static const CubeSetID * m_lowMap;
  static const CubeSetID * m_highMap; 

  CubeSet();
  ~CubeSet();

public:
  static void Instance() 
  { 
    if ( Singleton == nullptr ) Singleton = new CubeSet;
  }
  static void OnExit()
  {
    delete Singleton;
    Singleton = nullptr;
  }
  // create a new CubeID set generated by an original set rotated with a given CubeID
  // technically, the cache splitted into two 12-bit wide sequences
  static CubeSetID GetCubeSetID ( const CubeSetID sid, const CubeID cid )
  {
    return m_lowMap[ 24 * ( sid & 4095 ) + cid ] | m_highMap[ 24 * ( sid >> 12 ) + cid ];
  }
};

#endif  //  ! FINAL_STATES_CACHE__H
