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

template< cube_size N >
static void Init()
{
  GenerateRotationSet<N>::Instance();
           CPositions<N>::Instance();
}

template< cube_size N >
static void Exit()
{
           CPositions<N>::OnExit();
  GenerateRotationSet<N>::OnExit();
}
void exit()
{
  Exit <2> ();
  Exit <3> ();
  Exit <4> ();
  Exit <5> ();
  
  Projection::OnExit();
  CubeSet::OnExit();
  Simplex::OnExit();
}

void init()
{
  Simplex::Instance();
  CubeSet::Instance();
  Projection::Instance();
 
  Init <2> ();
  Init <3> ();
  Init <4> ();
  Init <5> ();
  
  std::atexit( exit );
}

