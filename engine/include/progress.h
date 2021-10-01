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
  
  DistID    m_depth;
  DistID    m_maxDepth;
  BitMap  * m_gradientStack;
  BitMap  * m_gradient;

  RotID   * m_solutionStack;
  RotID   * m_solution;

public:
Progression()
 :  m_depth( 0 )
 ,  m_gradientStack ( new BitMap [ StackSize ] {} )
 ,  m_solutionStack ( new RotID  [ StackSize ] {} )
{
  reset();
}

virtual ~Progression()
{
  delete[] m_gradientStack;
  delete[] m_solutionStack;
}

DistID depth() const
{
  return m_depth;
}

void depth( DistID depth )
{
  m_depth = depth;
}

void push( const BitMap32ID gradient, const RotID rotID )
{
  -- m_depth;
  ( ++ m_gradient ) -> set( gradient );
  *( m_solution ++ )  = rotID;
}

void pop()
{
  ++ m_depth;
  -- m_gradient;
  -- m_solution;
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

void setNode( BitMapID gradient )
{
  return m_gradient -> set( gradient );
}
  
void solution( Sequence & solution ) const
{
  solution.set( m_solutionStack, m_solution - m_solutionStack );
}

void reset()
{
  m_gradient = m_gradientStack;
  m_solution = m_solutionStack;
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

};

#endif  //  ! PROGRESS__H