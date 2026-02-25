#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "grid.h"
#include "mouse.h"
#include <queue>

enum class Command { MoveForward, TurnLeft, TurnRight };

class Simulator {
public:
  Simulator(int width, int height);

  void step();

  Grid &getGrid() noexcept;
  const Grid &getGrid() const noexcept;

  Mouse &getMouse() noexcept;
  const Mouse &getMouse() const noexcept;

  void enqueueCommand(Command cmd);
private:
  Grid grid;
  Mouse mouse;
  std::queue<Command> commandQueue;
};

#endif