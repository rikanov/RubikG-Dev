#ifndef PROGRESS__H
#define PROGRESS__H

#include <base_types.h>
#include <bitmap_set.h>
#include <sequence.h>
#include <text_output.h>

class Progression
{
private:
  static constexpr size_t StackSize = 20;
  
  DistID       m_depth;
  DistID       m_maxDepth;
  BitMap     * m_gradientStack;
  BitMap     * m_gradient;

  BitMapID   * m_gradientStore;
  BitMapID   * m_gradientOrig;

  BitMap32ID * m_targetStack;
  BitMap32ID * m_target;

  RotID      * m_solutionStack;
  RotID      * m_solution;

public:
Progression()
 :  m_depth( 0 )
 ,  m_gradientStack ( new BitMap     [ StackSize ] {} )
 ,  m_gradientStore ( new BitMapID   [ StackSize ] {} )
 ,  m_targetStack   ( new BitMap32ID [ StackSize ] {} )
 ,  m_solutionStack ( new RotID      [ StackSize ] {} )
{
}

DistID depth() const
{
  return m_depth;
}

void depth( DistID depth )
{
  m_depth = depth;
}

void initProgression( const BitMapID gradient, const BitMap32ID target )
{
  m_gradient     = m_gradientStack;
  m_gradientOrig = m_gradientStore;
  m_target       = m_targetStack;
  m_solution     = m_solutionStack;

   m_gradient    -> set( gradient );
  *m_gradientOrig = gradient;
  *m_target       = target;
}

void push( const BitMapID gradient, const BitMap32ID target, const RotID rotID )
{
  -- m_depth;
   ( ++ m_gradient )    -> set( gradient );
  *( ++ m_gradientOrig ) = gradient;
  *( ++ m_target   )     = target;
  *( m_solution ++ )     = rotID;
}

void pop()
{
  ++ m_depth;
  -- m_gradient;
  -- m_gradientOrig;
  -- m_target;
  -- m_solution;
}

BitMap32ID lastTarget() const
{
  return *m_target;
}

bool onTheTop() const
{
  return m_depth == m_maxDepth;
}

bool onEmptyNode() const
{
  return m_gradient -> empty();
}

bool inSolvedState() const
{
  return m_depth == 0 && m_gradient -> next() == 0;
}

bool chooseBranch( RotID & next )
{
  return m_gradient -> next( next );
}
  
void solution( Sequence & solution ) const
{
  solution.set( m_solutionStack, m_solution - m_solutionStack );
}

void startSearch()
{
  clog_( "max depth:" );
  m_depth = 0;
  m_maxDepth = 0;
}

bool heightLessThan( const DistID distID ) const
{
  return distID > m_maxDepth;
}

void deepening()
{
  ++ m_depth;
  ++ m_maxDepth;
  clog_( "..", (int) m_maxDepth );
}

template< cube_size N >
void printResult() const
{
  for ( DistID step = 0; step <= m_maxDepth; ++ step )
  {
    clog_( CRotations<N>::ToString( m_solutionStack[ step ] ) );
    BitMap::Print_( m_targetStack[ step ], 24, 8 );
    clog_( ' ' );
    BitMap::Print_( m_gradientStore[ step ], 9 * N + 1, 3 * N );
    clog_( ' ' );
    m_gradientStack[ step ].print( 9 * N + 1, 3 * N );
  }
}

~Progression()
{
  delete[] m_gradientStack;
  delete[] m_gradientStore;
  delete[] m_targetStack;
  delete[] m_solutionStack;
}

};

#endif  //  ! PROGRESS__H