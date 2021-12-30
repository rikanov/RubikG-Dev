#include <rubik.h>

template< cube_size N >
bool Rubik<N>::integrity() const
{
  return true; // ToDo
}

template< cube_size N >
size_t Rubik<N>::shuffle( const size_t depth )
{
  Stack<RotID>::reset();
  static std::random_device randomDevice;
  static std::default_random_engine engine( randomDevice() );
  static std::uniform_int_distribution<int> dist( 2 * N * N, 3 * N * N);
  int counter = depth == 0 ? dist( engine ) : depth;
  while ( 0 < counter-- )
  {
    Rubik<N>::rotate( CRotations<N>::Random() );
  }
  return Stack<RotID>::size();
}

