#ifndef FACTORY_TREE__H
#define FACTORY_TREE__H

#include <base_types.h>
#include <acceptance.h>


template< cube_size N >
class Factory
{
protected:

  // Array
 // ------------------------
  template<typename Type>
  class Array;

  // Patch factory
 // ------------------------
  class Patch;
  class PatchAPI;

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
  class Progress;
  void create( const size_t, const PosID *, AcceptFunction af = Accept<N>::Normal );
};

#include <factory_array.cpp>

#include <factories/patch_factory.cpp>
#include <APIs/patch_api.cpp>

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

#include <factory_tree_create.cpp>
#include <factory_progress.cpp>

#endif  //  ! FACTORY_TREE__H
