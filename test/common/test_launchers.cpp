#include <test.h>

bool UnitTests::testCore() const
{
  bool result = true;
  result &= unit_Simplex();
  result &= unit_CubeSet();
  result &= unit_CRotations();
  result &= unit_CExtRotations();
  result &= unit_CGenRotations();
  result &= unit_Projection();
  result &= unit_Rubik();
  return result;
}

bool UnitTests::testEngine() const
{
  bool result = true;
  result &= unit_Insight();
  result &= unit_Subgroup();
  result &= unit_Engine();
  return result;
}

bool UnitTests::testAI() const
{
  bool result = true;
  result &= unit_Resolver();
  return result;
}
