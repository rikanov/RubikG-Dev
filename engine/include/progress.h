#ifndef PROGRESS__H
#define PROGRESS__H

#include <base_types.h>

class BitMap;
class Sequence;

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
  Progression();
  ~Progression();

  BitMap32ID lastTarget() const;
  DistID depth() const;

  void depth( DistID depth );
  void initProgression( const BitMapID gradient, const BitMap32ID target );
  void startSearch();
  void deepening();
  void push( const BitMapID gradient, const BitMap32ID target, const RotID rotID );
  void pop();
  void noSolution();
  void solution( Sequence & solution ) const;
  bool chooseBranch( RotID & next );

  bool onTheTop() const;
  bool onEmptyNode() const;
  bool inSolvedState() const;
  bool heightLessThan( const DistID distID ) const;

  template< cube_size N >
  void printResult( const bool whole = false ) const;
};

// add template
#include "../source/progress_result_print.cpp"
#endif  //  ! PROGRESS__H
