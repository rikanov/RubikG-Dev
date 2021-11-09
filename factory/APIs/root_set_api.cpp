#ifndef API_ROOT_SET__TPP
#define API_ROOT_SET__TPP

#include <qeueu.h>
#include <factory_tree.h>

template< cube_size N >
class Factory<N>::RootSetAPI: public Factory<N>::RootSet
{

public:
  RootSetAPI() = default;
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
Factory<N>::RootSetAPI::RootSetAPI( const RootSet & rs )
  : RootSet( rs )
{
}

template< cube_size N >
Factory<N>::RootSetAPI::RootSetAPI( const size_t size, const PosID * pos, AcceptFunction af )
  : RootSet( size, pos, af )
{
}

template< cube_size N >
void Factory<N>::RootSetAPI::toQeueu( Qeueu & qeu ) const
{
  for ( size_t i = 0; i < this -> m_numberOfRoots; ++ i )
  {
    qeu << RootSet::m_setOfRoots.get()[i];
  }
}

template< cube_size N >
void Factory<N>::RootSetAPI::setRoots( std::function<void (const GroupID)> set ) const
{
  for ( size_t i = 0; i < this -> m_numberOfRoots; ++ i )
  {
    set( RootSet::m_setOfRoots.get()[i] );
  }
}

#endif  //  ! API_ROOT_SET__TPP
