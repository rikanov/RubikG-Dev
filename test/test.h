#ifndef UNIT_TEST__H
#define UNIT_TEST__H

#include <string>
#include <text_output.h>

class UnitTests
{
private:
  
  static void head   ( const std::string& unit );
  static void tcase  ( const std::string& unit, const std::string& msg1 = "", const std::string& msg2 = "" );
  static void tail   ( const std::string& unit, bool success );
  static void finish ( const std::string& unit, bool success );
  static void stamp  ( bool success, bool& result );
  static void done   ();
  
   // Unit tests of base classes (core folder)
  // -----------------------------------------
  bool unit_Simplex()    const; // simplex.h
  bool unit_OCube()      const; // oriented_cube.h
  bool unit_CPositions() const; // cube_positions.h
  bool unit_CRotations() const; // cube_rotations.h
  bool unit_Rubik()      const; // rubik.h
  bool unit_CState()     const; // cube_state.h
  bool unit_Engine()     const; // engine.h 
  
public:
  bool testCore() const;
  
};

#endif // ! UNIT_TEST__H
