#ifndef ___ROOT_SET_API_H
#define ___ROOT_SET_API_H

#include <qeueu.h>
#include <factories/root_set_factory.h>

template< cube_size N >
class GuideFactory<N>::RootSetAPI: public GuideFactory<N>::RootSet
{

public:
  RootSetAPI();
  RootSetAPI( const RootSet & rs );
  RootSetAPI( const size_t size, const PosID * pos, AcceptFunction af = Accept<N>::Normal );

  void toQeueu( Qeueu & qeu ) const;
  void setRoots( std::function<void( const GroupID )> set ) const;
  BitMap32ID acceptedPriorStates() const
  {
    return RootSet::m_allowedPriors;
  }
};


template< cube_size N >
GuideFactory<N>::RootSetAPI::RootSetAPI()
  : RootSet()
{
}

template< cube_size N >
GuideFactory<N>::RootSetAPI::RootSetAPI( const size_t size, const PosID * pos, AcceptFunction af )
  : RootSet( size, pos, af )
{
}

template< cube_size N >
void GuideFactory<N>::RootSetAPI::toQeueu( Qeueu & qeu ) const
{
  for ( size_t i = 0; i < this -> m_numberOfRoots; ++ i )
  {
    qeu << RootSet::m_setOfRoots[i];
  }
}

template< cube_size N >
void GuideFactory<N>::RootSetAPI::setRoots( std::function<void (const GroupID)> set ) const
{
  for ( size_t i = 0; i < this -> m_numberOfRoots; ++ i )
  {
    set( RootSet::m_setOfRoots[i] );
  }
}

#endif  //  ! ___ROOT_SET_API_H
