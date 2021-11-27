#ifndef ___SIGNPOST__H
#define ___SIGNPOST__H

#include <bitmap_set.h>

class Signpost
{
  BitMap m_gradient;
  BitMap m_target;

public:
  void set( const BitMapID gradient, const BitMap32ID target )
  {
    m_gradient.set( gradient );
    m_target.set( target );
  }

  void restrict( const Signpost * sp )
  {
    m_gradient.restrict( sp -> m_gradient );
    m_target.restrict( sp -> m_target );
  }

  void restrict( const BitMapID gradient, const BitMap32ID target )
  {
    m_gradient.restrict( gradient );
    m_target.restrict( target );
  }

  void expand( const Signpost * sp  )
  {
    m_gradient.expand( sp -> m_gradient );
    m_target.expand( sp -> m_target );
  }

  void expand( const BitMapID gradient, const BitMap32ID target )
  {
    m_gradient.expand( gradient );
    m_target.expand( target );
  }

  bool deadEnd() const
  {
    return m_gradient.empty() || m_target.empty();
  }

  bool operator>> ( RotID & next)
  {
    return m_gradient >> next;
  }

  BitMap32ID getTargetID() const
  {
    return m_target.data();
  }

};

#endif  //  ! ___SIGNPOST__H
