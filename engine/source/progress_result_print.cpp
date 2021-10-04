#include <progress.h>
#include <cube_rotations.h>
#include <bitmap_set.h>
#include <text_output.h>


template< cube_size N >
void Progression::printResult( const bool whole ) const
{
  for ( DistID step = 0; step <= ( whole ? m_maxDepth : m_depth ); ++ step )
  {
    clog_( CRotations<N>::ToString( m_solutionStack[ step ] ) );
    BitMap::Print_( m_targetStack[ step ], 24, 8 );
    clog_( ' ' );
    BitMap::Print_( m_gradientStore[ step ], 9 * N + 1, 3 * N );
    clog_( ' ' );
    m_gradientStack[ step ].print( 9 * N + 1, 3 * N );
  }
}
