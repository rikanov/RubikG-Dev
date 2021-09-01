#ifndef UNIT_TEST__H
#define UNIT_TEST__H

#include <string>
#include <text_output.h>

class UnitTests
{
private:
public:
 // Common
//--------
  static void head   ( const std::string& unit );
  static void tcase  ( const std::string& unit, const std::string& msg1 = "", const std::string& msg2 = "" );
  static void tail   ( const std::string& unit, bool success );
  static void finish ( const std::string& unit, bool success );
  static void stamp  ( bool success, bool& result );
  static void done   ();
  
 // Timer
//-------
  static void   timerON  ();
  static void   timerOFF ();
  static double ellapsed ();

 // Random generator
//------------------
  static int random( int a, int b );
  
private:
   // Unit tests of base classes (core folder)
  //------------------------------------------
  bool unit_Simplex()       const; // core/simplex.h
  bool unit_CubeSet()       const; // cube_set.h
  bool unit_OCube()         const; // core/oriented_cube.h
  bool unit_CPositions()    const; // core/cube_positions.h
  bool unit_CRotations()    const; // core/cube_rotations.h
  bool unit_CExtRotations() const; // core/cube_rotations.h
  bool unit_Projection()    const; // core/projection.h
  bool unit_Rubik()         const; // core/rubik.h
   
   // Unit tests of engines (engine folder)
  //---------------------------------------
  bool unit_Insight()       const; // engine/insight.h 
  bool unit_RawMap()        const; // engine/raw_map.h
  bool unit_StateMap()      const; // engine/state_map.h
  bool unit_Engine()        const; // engine/engine.h
public:
  bool testCore()   const;
  bool testEngine() const;
  
};

#endif // ! UNIT_TEST__H
