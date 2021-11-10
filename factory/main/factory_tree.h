#ifndef FACTORY_TREE__H
#define FACTORY_TREE__H

#include <base_types.h>
#include <acceptance.h>

template< cube_size N >
class Factory
{
  
public:

  Rubik<N>      * cube;
  size_t          patternSize;
  CubeID          patternTrans;
  const PosID   * pattern;
  AcceptFunction  accept;

protected:

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
public:
  // Evaluator factory
 // ------------------------
  class Evaluator;
  class EvaluatorAPI;

public:

  class Task;

  Factory();
  Task * get();
};

#include <factories/pattern_factory.cpp>
#include <APIs/pattern_api.cpp>

#include <factories/generator_factory.cpp>
#include <APIs/generator_api.cpp>

#include <factories/subgroup_factory.cpp>
#include <APIs/subgroup_api.cpp>

#include <factories/root_set_factory.cpp>
#include <APIs/root_set_api.cpp>

#include <factories/connection_rules_factory.cpp>
#include <APIs/connection_rules_api.cpp>

#include <factories/evaluator_factory.cpp>
#include <APIs/evaluator_api.cpp>

#include <factory_task.cpp>
#include <factory_tree_create.cpp>

#endif  //  ! FACTORY_TREE__H
