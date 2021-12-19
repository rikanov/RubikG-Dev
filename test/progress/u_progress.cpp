#include <test.h>
#include <progress.h>

template< cube_size N >
static bool sequenceOK( const Sequence & seq, const size_t from )
{
  using _crot = CRotations<N>;
  if ( seq.size() < from + 2 )
  {
    return true;
  }
  bool result = true;
  size_t counter = 0;
  for ( auto P = seq.begin() + from; P != seq.end() - 1; ++ P )
  {
    clog_( P - seq.begin() - from, '.', _crot::ToString( *P ) );
    if ( *P == *( P + 1 ) )
    {
      clog_( Color::red, " ERROR: repeating rotation", Color::off );
      result = false;
      continue;
    }
    const bool equalAxis  = _crot::GetAxis  ( *P ) == _crot::GetAxis  ( *( P + 1 ) );
    const bool equalLayer = _crot::GetLayer ( *P ) == _crot::GetLayer ( *( P + 1 ) ) || 2 == N;
    const bool equalTurn  = _crot::GetTurn  ( *P ) == _crot::GetTurn  ( *( P + 1 ) );
    if ( equalAxis && equalLayer )  // Axes and Layers equal
    {
      clog_( Color::red, " ERROR: axis & layer equality", Color::off );
      result = false;
    }
    counter = equalAxis && equalTurn ? counter + 1 : 0;  // inrease if Axes and Turns are equal;
    if ( counter > 0 && N - 2 == counter )
    {
      clog_( Color::red, " ERROR: unnecessary transposition", Color::off );
      result = false;
    }
    NL();
  }
  return result;
}
bool UnitTests::unit_Progress() const
{
  bool success = true;
  head( "Progress" );

  tcase( "Cube size 2" );
  bool ok2 = true;
  {
    constexpr size_t N = 2;

    const Pattern<N> left = {
      CPositions<N>::GetPosID( 0, 0, 0 ),
      CPositions<N>::GetPosID( 0, 0, 1 ),
      CPositions<N>::GetPosID( 0, 1, 1 ),
      CPositions<N>::GetPosID( 0, 1, 0 ),
    };

    const Pattern<N> right = {
      CPositions<N>::GetPosID( 1, 0, 0 ),
      CPositions<N>::GetPosID( 1, 0, 1 ),
      CPositions<N>::GetPosID( 1, 1, 1 ),
      CPositions<N>::GetPosID( 1, 1, 0 ),
    };

    Rubik<N> testCube2;
    const size_t step = testCube2.shuffle();
    testCube2.print();
    Progress<N> test2;

    test2.addGuide( _Scheduled, left );
    test2.addGuide( _Scheduled, right );
    test2.next();

    test2.solve( testCube2 );

    ok2 = sequenceOK<2>( testCube2, step );
    stamp( ok2, success );
  }


  tcase( "Cube size 3" );
  bool ok3 = true;
  {
    constexpr size_t N = 3;

    const Pattern<N> cross = {
      CPositions<N>::GetPosID( 1, 2, 2 ),
      CPositions<N>::GetPosID( 0, 1, 2 ),
      CPositions<N>::GetPosID( 1, 1, 2 ),
      CPositions<N>::GetPosID( 2, 1, 2 ),
      CPositions<N>::GetPosID( 1, 0, 2 )
    };

    const Pattern<N> bind = {
      CPositions<N>::GetPosID( 1, 0, 1 ),
      CPositions<N>::GetPosID( 1, 0, 2 ),
    };
    const Pattern<N> block1 = {
      CPositions<N>::GetPosID( 2, 0, 2 ),
      CPositions<N>::GetPosID( 2, 0, 1 ),
      CPositions<N>::GetPosID( 1, 0, 2 )
    };
    const Pattern<N> block2 = {
      CPositions<N>::GetPosID( 2, 2, 2 ),
      CPositions<N>::GetPosID( 2, 2, 1 ),
      CPositions<N>::GetPosID( 1, 2, 2 )
    };
    const Pattern<N> block3 = {
      CPositions<N>::GetPosID( 0, 2, 2 ),
      CPositions<N>::GetPosID( 0, 2, 1 ),
      CPositions<N>::GetPosID( 0, 1, 2 ),
    };
    const Pattern<N> block4 = {
      CPositions<N>::GetPosID( 0, 0, 2 ),
      CPositions<N>::GetPosID( 0, 0, 1 ),
      CPositions<N>::GetPosID( 1, 0, 2 ),
    };

    const Pattern<N> cross2 = {
      CPositions<N>::GetPosID( 1, 2, 0 ),
      CPositions<N>::GetPosID( 0, 1, 0 ),
      CPositions<N>::GetPosID( 1, 1, 0 ),
      CPositions<N>::GetPosID( 2, 1, 0 ),
      CPositions<N>::GetPosID( 1, 0, 0 ),
    };

    const Pattern<N> corners = {
      CPositions<N>::GetPosID( 0, 0, 0 ),
      CPositions<N>::GetPosID( 0, 2, 0 ),
      CPositions<N>::GetPosID( 1, 1, 0 ),
      CPositions<N>::GetPosID( 2, 0, 0 ),
      CPositions<N>::GetPosID( 2, 2, 0 ),
    };

    Rubik<N> testCube;
    const size_t step = testCube.shuffle();
    testCube.print();

    Progress<N> test;

    test.addGuide( _Scheduled, cross );
    test.addGuide( _Scheduled, bind );
    test.addGuide( _Optional,  block1 );
    test.addGuide( _Optional,  block2 );
    test.addGuide( _Optional,  block3 );
    test.addGuide( _Optional,  block4 );
    test.next();

    test.addGuide( _Scheduled, cross2, Accept<N>::RotAxis( _Z ) );
    test.addGuide( _Scheduled, corners, Accept<N>::RotAxis( _Z ) );
    test.next();

    test.addGuide( _Optional, cross2 );
    test.addGuide( _Optional, corners );
    test.next();

    test.solve( testCube );
    testCube.save( "test_3.rub" );
    ok3 = sequenceOK<3>( testCube, step );
    stamp( ok3, success );
  }

  finish( "Progress", success );

  return success;
}

