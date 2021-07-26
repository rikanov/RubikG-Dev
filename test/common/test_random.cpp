#include <test.h>
#include <random>

int UnitTests::random( int a, int b )
{
 std::random_device                 randomDevice;
 std::default_random_engine         randomEngine( randomDevice() );
 std::uniform_int_distribution<int> distribution( a, b ); 

 return distribution( randomEngine );
}