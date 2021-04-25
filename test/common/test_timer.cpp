#include <test.h>
#include <ctime>

static clock_t _StartTime;
static clock_t _EndTime;

void UnitTests::timerON()
{
  _StartTime = clock();
}

void UnitTests::timerOFF()
{
  _EndTime = clock();
}

double UnitTests::ellapsed()
{
  return ( _EndTime - _StartTime ) / (double) CLOCKS_PER_SEC;
}
