#include <progress.h>
#include <sequence.h>

Progression::Progression()
 :  m_depth( 0 )
 ,  m_gradientStack ( new BitMap     [ StackSize ] {} )
 ,  m_gradientStore ( new BitMapID   [ StackSize ] {} )
 ,  m_targetStack   ( new BitMap32ID [ StackSize ] {} )
 ,  m_solutionStack ( new RotID      [ StackSize ] {} )
{
}

DistID Progression::depth() const
{
  return m_depth;
}

void Progression::depth( DistID depth )
{
  m_depth = depth;
}

void Progression::initProgression( const BitMapID gradient, const BitMap32ID target )
{
  for ( DistID depth = 0; depth <= m_maxDepth; ++ depth )
  {
    m_gradientStack[ depth ].set( 0 );
    m_gradientStore[ depth ] = 0;
    m_targetStack  [ depth ] = 0;
    m_solutionStack[ depth ] = 0;
  }

  m_gradient     = m_gradientStack;
  m_gradientOrig = m_gradientStore;
  m_target       = m_targetStack;
  m_solution     = m_solutionStack;

   m_gradient    -> set( gradient );
  *m_gradientOrig = gradient;
  *m_target       = target;
}

void Progression::push( const BitMapID gradient, const BitMap32ID target, const RotID rotID )
{
  -- m_depth;
   ( ++ m_gradient )    -> set( gradient );
  *( ++ m_gradientOrig ) = gradient;
  *( ++ m_target   )     = target;
  *( m_solution ++ )     = rotID;
}

void Progression::pop()
{
  ++ m_depth;
  -- m_gradient;
  -- m_gradientOrig;
  -- m_target;
  -- m_solution;
}

BitMap32ID Progression::lastTarget() const
{
  return *m_target;
}

void Progression::noSolution()
{
  m_gradient -> remove( 0 );
}

bool Progression::onTheTop() const
{
  return m_depth == m_maxDepth;
}

bool Progression::onEmptyNode() const
{
  return m_gradient -> empty();
}

bool Progression::inSolvedState() const
{
  if ( m_depth > m_maxDepth )
  {
    clog( (int) m_depth );
  }
  return m_depth == 0 && m_gradient -> contains( 0 );
}

bool Progression::chooseBranch( RotID & next )
{
  return m_gradient -> next( next );
}

void Progression::solution( Sequence & solution ) const
{
  solution.set( m_solutionStack, m_solution - m_solutionStack );
}

void Progression::startSearch()
{
  clog_( "max depth:" );
  m_depth = 0;
  m_maxDepth = 0;
}

bool Progression::heightLessThan( const DistID distID ) const
{
  return distID > m_maxDepth;
}

void Progression::deepening()
{
  ++ m_depth;
  ++ m_maxDepth;
  clog_( "..", (int) m_maxDepth );
}

Progression::~Progression()
{
  delete[] m_gradientStack;
  delete[] m_gradientStore;
  delete[] m_targetStack;
  delete[] m_solutionStack;
}
