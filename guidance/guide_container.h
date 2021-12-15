#ifndef ___GUIDE_CONTAINER__H
#define ___GUIDE_CONTAINER__H

#include<guide.h>
#include<dynamic_stack.h>

template < cube_size N >
class GuideContainer: protected Stack< typename GuideFactory<N>::Guide >
                    , protected Stack< typename GuideFactory<N>::Guide *>
{
  static constexpr size_t MaxNumberOfGuides = 200;

  using Guide = typename GuideFactory<N>::Guide;
  using Guides = Stack<Guide>;
  using Sequence = Stack<Guide *>;

public:
  GuideContainer();
  void add( const Guide & );
  void next();
  void revert();
};

template< cube_size N >
GuideContainer<N>::GuideContainer()
  : Guides( MaxNumberOfGuides )
  , Sequence( MaxNumberOfGuides )
{
  Sequence::push( Guides::begin() );
}

template< cube_size N >
void GuideContainer<N>::add( const Guide & guide )
{
  Guides::push( guide );
}

template< cube_size N >
void GuideContainer<N>::next()
{
  Sequence::push( Guides::next() );
}

template< cube_size N >
void GuideContainer<N>::revert()
{
  Guides::set( Sequence::pop() );
}

#endif  //  ! ___GUIDE_CONTAINER__H
