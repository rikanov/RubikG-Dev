#include <test.h>

bool UnitTests::testCore() const
{
  bool result = true;
  result &= unit_Simplex();
  result &= unit_CubeSet();
  result &= unit_CRotations();
  result &= unit_CGenRotations();
  result &= unit_Rubik();
  return result;
}

bool UnitTests::testEngine() const
{
  bool result = true;
  result &= unit_Progress();
  return result;
}
