#pragma once

#include <iostream>
#include <cstdlib>

#include <test.h>
#include <cli.h>
#include <bitmap_set.h>
#include <cube_rotations.h>
#include <cube_set.h>
#include <rubik.h>


void exit()
{
  CPositions<2>::OnExit();
  CPositions<3>::OnExit();
  CPositions<4>::OnExit();
  CPositions<5>::OnExit();
  CubeSet::OnExit();
  Simplex::OnExit();
}

void init()
{
  Simplex::Instance();
  CubeSet::Instance();
  CPositions<2>::Instance();
  CPositions<3>::Instance();
  CPositions<4>::Instance();
  CPositions<5>::Instance();
  std::atexit( exit );
}

