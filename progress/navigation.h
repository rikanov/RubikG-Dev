#ifndef ___PROGRESS_NAVIGATION__H
#define ___PROGRESS_NAVIGATION__H

#include <guide.h>

template< cube_size N >
class Navigation
{
  static constexpr size_t MaxNumberOfGuides = N * 10;
  using Guide = typename GuideFactory<N>::Guide;

  Guide * m_optional;
  Guide * m_optionalEnd;

  Guide * m_scheduled;
  Guide * m_scheduledEnd;

public:

  Navigation()
    : m_optional  ( new Guide[ MaxNumberOfGuides ] )
    , m_scheduled ( new Guide[ MaxNumberOfGuides ] )
  {
    m_optionalEnd  = m_optional;
    m_scheduledEnd = m_scheduled;
  }

  void addOptional ( Guide guide )
  {
    *( m_optionalEnd ++ ) = guide;
  }

  void addScheduled( Guide guide )
  {
    *( m_scheduledEnd ++ ) = guide;
  }

  bool setNode( const RotID rotID )
  {
    for ( auto p = m_optional; p != m_optionalEnd; ++ p )
    {
      p -> expand( rotID );
    }

    for ( auto p = m_scheduled; p != m_scheduledEnd; ++ p )
    {
      if ( p -> restrict( rotID ) == false )
      {
        return false;
      }
    }
    return true;
  }

  virtual ~Navigation()
  {
    delete[] m_optional;
    delete[] m_scheduled;
  }
};

#endif  //  ! ___PROGRESS_NAVIGATION__H
