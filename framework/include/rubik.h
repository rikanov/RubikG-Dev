#ifndef ___CUBE_FRAMEWORK_HEADER
#define ___CUBE_FRAMEWORK_HEADER

#include <cube_rotations.h>
#include <cube_positions.h>
#include <sequence.h>

/// ----------------------------------- Template declarations starts here ------------------------------------- ///
template< cube_size N >
class Rubik: public Sequence
{
  static constexpr int Fsize = CPositions<N>::GetSize();
  CubeID * frameworkSpace;

  void setRotate( const RotID rotID );
  void rotate  ( const Axis, const Layer, const Turn turn = 1 );

public:
  
  // Constructors
  Rubik( void );
  Rubik( const Rubik<N>& );
  Rubik( Rubik<N>&& f );

  // Operations
  void reset   ();
  void refresh ();
  void history ( const size_t step );
  void revert  ( const size_t step );
  void rotate  ( const RotID rotID );
  void rotate  ( const Sequence & seq );

  size_t shuffle( size_t depth = 0 );
  
  // Operators
  bool operator== ( const Rubik<N>& X ) const;
  const Rubik<N>& operator=  ( const Rubik<N>& B );

  // Destructor
  ~Rubik( );
  
  // Query functions
  CubeID  getCubeID ( PosID id, RotID rot ) const;
  CubeID  getCubeID ( PosID id ) const;
  OCube   getCube   ( PosID id ) const;
  
  PosID whatIs  ( PosID id ) const;
  PosID whereIs ( PosID id ) const;
  Coord whatIs  ( Coord C )  const;
  Coord whereIs ( Coord C )  const;

  CubeID transpose ( PosID id, CubeID trans = 0 ) const ;

  bool    integrity ( void  )  const ;
  bool    isSolved  ( void  )  const;
  Orient  getOrient ( const Orient front, const Orient up, Layer x, Layer y ) const; // left-bottom corner: x = 0, y = 0

  // Printer
  void show ( Orient, Orient, Layer, Layer ) const;
  void show ( Orient ) const;
  void print( Orient, Orient ) const;
  void print( bool separator = true ) const;
};

// include template source
#include "../source/rubik.cpp"
#include "../source/rubik_print.cpp"

#endif  //  ! ___CUBE_FRAMEWORK_HEADER
