#include "simulator.h"

Simulator::Simulator(int width, int height)
    : grid(width, height), mouse(height - 1, 0) {

  grid.addWall(10, 5, Direction::EAST);
  grid.addWall(9, 5, Direction::SOUTH);
}

void Simulator::enqueueCommand(Command cmd) { commandQueue.push(cmd); }

void Simulator::step() {
  if (commandQueue.empty()) {
    return;
  }
  Command cmd = commandQueue.front();
  commandQueue.pop();
  switch (cmd) {
  case Command::MoveForward:
    mouse.moveForward(grid);
    break;
  case Command::TurnLeft:
    mouse.turnLeft();
    break;
  case Command::TurnRight:
    mouse.turnRight();
    break;
  }
}

Grid &Simulator::getGrid() noexcept { return grid; }
const Grid &Simulator::getGrid() const noexcept { return grid; }

Mouse &Simulator::getMouse() noexcept { return mouse; }
const Mouse &Simulator::getMouse() const noexcept { return mouse; }