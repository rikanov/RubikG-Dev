#ifndef FACTORY_TREE__H
#define FACTORY_TREE__H

#include <acceptance.h>

template< cube_size N >
class GuideFactory
{
  static size_t m_index;
  
protected:
public:
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
  class GuideBase;
  GuideFactory();
  static Guide getGuide( const size_t, const PosID *, AcceptFunction );

};

#include <factory.cpp>

#endif  //  ! FACTORY_TREE__H
