#ifndef FACTORY_TREE__H
#define FACTORY_TREE__H

#include <pattern.h>
#include <acceptance.h>

template< cube_size N >
class GuideFactory
{

public:
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

  class Guide;
  class GuideBase;

private:
  Stack<Guide> m_guides;

public:

  GuideFactory();
  Guide getGuide( const Pattern<N> &, AcceptFunction );

};

#include <factory.cpp>

#endif  //  ! FACTORY_TREE__H
