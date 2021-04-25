#pragma once

#include <iostream>
#include <cstdlib>

#include <test.h>
#include <cli.h>
#include <cube_rotations.h>
#include <rubik.h>


void exit()
{
  CPositions<2>::OnExit();
  CPositions<3>::OnExit();
  CPositions<4>::OnExit();
  CPositions<5>::OnExit();

  CRotations<2>::OnExit();
  CRotations<3>::OnExit();
  CRotations<4>::OnExit();
  CRotations<5>::OnExit();
  Simplex::OnExit();
}

void init()
{
  Simplex::Instance();
  CRotations<2>::Instance();
  CRotations<3>::Instance();
  CRotations<4>::Instance();
  CRotations<5>::Instance();

  CExtRotations<2>::Instance();
  CExtRotations<3>::Instance();
  CExtRotations<4>::Instance();
  CExtRotations<5>::Instance();

  CPositions<2>::Instance();
  CPositions<3>::Instance();
  CPositions<4>::Instance();
  CPositions<5>::Instance();
  std::atexit( exit );
}

