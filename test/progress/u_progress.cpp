#include <test.h>
#include <progress.h>


bool UnitTests::unit_Progress() const
{
  bool success = true;
  head( "Progress" );

  tcase( "Cube size 2" );
  {
    constexpr size_t N = 2;

    const Pattern<N> left = {
      CPositions<N>::GetPosID( 0, 0, 0 ),
      CPositions<N>::GetPosID( 0, 0, 1 ),
      CPositions<N>::GetPosID( 0, 1, 1 ),
      CPositions<N>::GetPosID( 0, 1, 0 ),
    };

    const Pattern<N> right = left * Simplex::Tilt( _Z, 2 );
    Rubik<N> testCube2;
    const size_t step = testCube2.shuffle();
    testCube2.print();
    Progress<N> test2;

    test2.addGuide( _Scheduled, left );
    test2.addGuide( _Scheduled, right );
    test2.next();

    test2.solve( testCube2 );

    stamp( test2.consistency, success );
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
      CPositions<N>::GetPosID( 1, 0, 2 ),
      CPositions<N>::GetPosID( 1, 0, 1 )
    };

    const Pattern<N> block = {
      CPositions<N>::GetPosID( 2, 0, 2 ),
      CPositions<N>::GetPosID( 2, 0, 1 ),
      CPositions<N>::GetPosID( 1, 0, 2 )
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
    clog( "shuffle steps:", step );
    testCube.print();

    Progress<N> test;

    timerON();
    const Pattern<N> block1 = block * Simplex::Tilt( _Z, 1 );
    const Pattern<N> block2 = block * Simplex::Tilt( _Z, 2 );
    const Pattern<N> block3 = block * Simplex::Tilt( _Z, 3 );
    test.addGuide( _Scheduled, cross );
    test.addGuide( _Optional,  block );
    test.addGuide( _Optional,  block1 );
    test.addGuide( _Optional,  block2 );
    test.addGuide( _Optional,  block3 );
    test.next();
    timerOFF();

    for ( int i = 1; i <= 100 && test.consistency; ++ i )
    {
      testCube.shuffle();
      test.reset();
      test.solve( testCube );
      cdraw( '=', 30 );
      clog_( "end of the", i, ". test case" );
      stamp( test.consistency, success );
    }

    const Pattern<N> cross2 = cross * Simplex::Tilt( _Y, 2 );

    test.addGuide( _Scheduled, cross2, Accept<N>::RotAxis( _Z ) );
    test.addGuide( _Scheduled, corners, Accept<N>::RotAxis( _Z ) );
    test.next( 10 );

    test.addGuide( _Optional, cross2 );
    test.addGuide( _Optional, corners );
    test.next( 10 );

    test.solve( testCube );

    BitMap restrict = {
      CRotations<N>::GetRotID( _Y, 0, 1 ),
      CRotations<N>::GetRotID( _Y, 0, 2 ),
      CRotations<N>::GetRotID( _Y, 0, 3 ),
      CRotations<N>::GetRotID( _Y, 2, 1 ),
      CRotations<N>::GetRotID( _Y, 2, 2 ),
      CRotations<N>::GetRotID( _Y, 2, 3 ),
      CRotations<N>::GetRotID( _X, 0, 1 ),
      CRotations<N>::GetRotID( _X, 0, 2 ),
      CRotations<N>::GetRotID( _X, 0, 3 ),
      CRotations<N>::GetRotID( _X, 2, 1 ),
      CRotations<N>::GetRotID( _X, 2, 2 ),
      CRotations<N>::GetRotID( _X, 2, 3 ),
      CRotations<N>::GetRotID( _Z, 0, 1 ),
      CRotations<N>::GetRotID( _Z, 0, 2 ),
      CRotations<N>::GetRotID( _Z, 0, 3 ),
      CRotations<N>::GetRotID( _Z, 2, 1 ),
      CRotations<N>::GetRotID( _Z, 2, 2 ),
      CRotations<N>::GetRotID( _Z, 2, 3 ),
    }; // ToDo: add corner first solution test
    stamp( test.consistency, success );
  }

    clog( ellapsed() );
  finish( "Progress", success );

  return success;
}

