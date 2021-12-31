#ifndef ___GUIDE_CONTAINER__H
#define ___GUIDE_CONTAINER__H

#include<guide.h>
#include<dynamic_stack.h>

template < cube_size N >
class GuideContainer: protected Stack< typename GuideFactory<N>::Guide >
                    , protected Stack< typename GuideFactory<N>::Guide *>
{
  static constexpr size_t MaxNumberOfGuides = 200;

  using Guide  = typename GuideFactory<N>::Guide;
  using Guides = Stack<Guide>;
  using GSeq   = Stack<Guide *>;

public:
  GuideContainer();
  void add( const Guide & );
  void nextSolution();

  void extend( Guides &, const size_t ) const;

  const Guide * begin() const
  {
    return Guides::begin();
  }
  const Guide * end() const
  {
    return Guides::end();
  }
  const Guide & top() const
  {
    return Guides::top();
  }
};

template< cube_size N >
GuideContainer<N>::GuideContainer()
  : Guides ( MaxNumberOfGuides )
  , GSeq   ( MaxNumberOfGuides )
{
  GSeq::push( Guides::begin() );
}

template< cube_size N >
void GuideContainer<N>::add( const Guide & guide )
{
  for ( auto P = Guides::begin(); P != Guides::end(); ++ P )
  {
    if ( guide.stricterThan( *P ) )
    {
      *P = guide;
      return;
    }
  }
  Guides::push( guide );
}

template< cube_size N >
void GuideContainer<N>::nextSolution()
{
  GSeq::push( Guides::end() );
}

template< cube_size N >
void GuideContainer<N>::extend( GuideContainer::Guides& guides, const size_t step ) const
{
  guides.push( GSeq::get( step ), GSeq::get( step + 1 ) );
}


#endif  //  ! ___GUIDE_CONTAINER__H
