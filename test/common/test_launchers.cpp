#include <test.h>

bool UnitTests::testCore() const
{
  bool result = true;
  result &= unit_Simplex();
  result &= unit_CRotations();
  result &= unit_Rubik();
  result &= unit_CState();
  return result;
}
