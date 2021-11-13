#ifndef FACTORY_TREE__H
#define FACTORY_TREE__H

#include <base_types.h>
#include <acceptance.h>

template< cube_size N >
class Factory
{
  
public:

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

  // Evaluator factory
 // ------------------------
  class Evaluator;
  class EvaluatorAPI;

  // State factory
  // ------------------------
  class StateFactory;

public:

  class StateAPI;

  class ControlPanel;
  class Worker;

  Factory();
  Worker get( ControlPanel & );
};

#include <factory.cpp>

#endif  //  ! FACTORY_TREE__H
