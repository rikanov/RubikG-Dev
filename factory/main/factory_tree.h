#ifndef FACTORY_TREE__H
#define FACTORY_TREE__H

#include<task.h>

template< cube_size N >
class GuideFactory
{
  
protected:

  size_t          m_patternSize;
  CubeID          m_patternTrans;
  const PosID   * m_pattern;
  AcceptFunction  m_accept;

  Signpost * m_stack;
  DistID     m_level;
  DistID     m_depth;

  // Patch factory
 // ------------------------
  class Pattern;
  class PatternAPI;

  // Group generator factory
 // -------------------------
  class GroupGenerator;
  class GroupGeneratorAPI;

  // Subgroup factory
 // -------------------------
  class Subgroup;
  class SubgroupAPI;

  // Node connections factory
 // --------------------------
  class Connection;
  class ConnectionAPI;

  // Set of root nodes factory
 // --------------------------
  class RootSet;
  class RootSetAPI;

  // Evaluator factory
 // ------------------------
  class Evaluator;
  class EvaluatorAPI;

public:

  class Guide;
  GuideFactory();
  Guide getGuide( Task<N> & ) const;

  Signpost * actual();
};

#include <factory.cpp>

#endif  //  ! FACTORY_TREE__H
