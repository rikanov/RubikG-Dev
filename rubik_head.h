#pragma once

#include <iostream>
#include <cstdlib>

#include <test.h>
#include <cli.h>
#include <cube_rotations.h>
#include <gen_rotation_set.h>
#include <cube_set.h>
#include <projection.h>
#include <rubik.h>


void exit()
{
  CPositions<2>::OnExit();
  CPositions<3>::OnExit();
  CPositions<4>::OnExit();
  CPositions<5>::OnExit();
  GenerateRotationSet<2>::OnExit();
  GenerateRotationSet<3>::OnExit();
  GenerateRotationSet<4>::OnExit();
  GenerateRotationSet<5>::OnExit();
  Projection::OnExit();
  CubeSet::OnExit();
  Simplex::OnExit();
}

void init()
{
  Simplex::Instance();
  CubeSet::Instance();
  Projection::Instance();
  GenerateRotationSet<2>::Instance();
  GenerateRotationSet<3>::Instance();
  GenerateRotationSet<4>::Instance();
  GenerateRotationSet<5>::Instance();
  CPositions<2>::Instance();
  CPositions<3>::Instance();
  CPositions<4>::Instance();
  CPositions<5>::Instance();
  std::atexit( exit );
}

