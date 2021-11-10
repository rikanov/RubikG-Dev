#ifndef FACTORY_TASK__TPP
#define FACTORY_TASK__TPP

#include <rubik.h>
#include <acceptance.h>
#include <factory_tree.h>

template< cube_size N >
class Factory<N>::Task:  public Factory<N>::EvaluatorAPI
{
  Rubik<N>     * m_rubik;
  const CubeID   m_patternTrans;

public:
  Task( Rubik<N> * rubik, const size_t size, const CubeID trans, const PosID * pos, AcceptFunction af )
    : EvaluatorAPI( size, pos, af )
    , m_rubik( rubik )
    , m_patternTrans( trans )
  {
  }

  void setCube( Rubik<N> * rubik )
  {
    m_rubik = rubik;
  }
  
};
#endif  //  ! FACTORY_TASK__TPP