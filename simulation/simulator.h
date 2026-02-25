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

  void startFloodFill(int goalRow, int goalCol);
  void floodStep();

  void startNavigation() { navigateActive = true; }
  void stopNavigation() { navigateActive = false; }

  void navigateStep();

private:
  Grid grid;
  Mouse mouse;
  std::queue<Command> commandQueue;
  std::queue<std::pair<int, int>> floodQueue;
  bool floodActive = false;
  bool navigateActive = false;
};

#endif