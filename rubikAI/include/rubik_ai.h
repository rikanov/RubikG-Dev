#ifndef RUBIK_AI__H
#define RUBIK_AI__H

#include <engine.h>
#include <rubik.h>
#include <sequence.h>

template< cube_size N >
class RubikAi
{
  Engine<N> * m_engine;

public:
  RubikAi();

  // iteratively deepening algorithm ( IDA )
  Sequence solve();

};

#endif  // ! RUBIK_AI__H
