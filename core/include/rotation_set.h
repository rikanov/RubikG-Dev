#ifndef ROTATION_SET__H
#define ROTATION_SET__H

#include <base_types.h>

typedef uint64_t RotSetID;

class RotationSet
{
  const   RotSetID m_rotSetID;
  mutable RotSetID m_nextSubsetID;
  mutable RotID    m_rotID;

public:
  RotationSet( const RotSetID rs ): m_rotSetID( rs ), m_nextSubsetID(0), m_rotID(0) {}
  RotID start() const
  {
    m_nextSubsetID = m_rotSetID;
    return next();
  }

  RotSetID getSetID() const
  {
    return m_rotSetID;
  }

  RotID next() const
  {
    if ( m_nextSubsetID == 0 )
      return 0;
    
    // skip empty bits
    for( ; m_nextSubsetID ^1 ; m_nextSubsetID >>= 1, ++ m_rotID );

    return m_rotID ++;
  }

  static inline void Add ( RotSetID & rs, const RotID rotID )
  {
    rs |= ( 1ULL << rotID );
  }
};


#endif  //  ! ROTATION_SET__H
