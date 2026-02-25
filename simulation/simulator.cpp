#include "simulator.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <stack>

Simulator::Simulator(int width, int height)
    : grid(width, height), mouse(height - 1, 0) {
  // Build a full-wall grid first (outer walls already exist), then carve a
  // perfect maze. This guarantees coverage of every cell on a 16x16 grid.
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      if (c < width - 1)
        grid.addWall(r, c, Direction::EAST);
      if (r < height - 1)
        grid.addWall(r, c, Direction::SOUTH);
    }
  }

  std::vector<std::vector<bool>> visited(height,
                                         std::vector<bool>(width, false));
  std::stack<std::pair<int, int>> cellStack;
  std::mt19937 rng(std::random_device{}());

  auto inBounds = [&](int r, int c) {
    return r >= 0 && r < height && c >= 0 && c < width;
  };

  // Start where the mouse starts.
  int startRow = height - 1;
  int startCol = 0;
  visited[startRow][startCol] = true;
  cellStack.push({startRow, startCol});

  while (!cellStack.empty()) {
    auto [r, c] = cellStack.top();

    std::vector<Direction> directions = {Direction::NORTH, Direction::EAST,
                                         Direction::SOUTH, Direction::WEST};
    std::shuffle(directions.begin(), directions.end(), rng);

    bool moved = false;
    for (Direction dir : directions) {
      int nr = r;
      int nc = c;

      switch (dir) {
      case Direction::NORTH:
        nr -= 1;
        break;
      case Direction::EAST:
        nc += 1;
        break;
      case Direction::SOUTH:
        nr += 1;
        break;
      case Direction::WEST:
        nc -= 1;
        break;
      }

      if (!inBounds(nr, nc) || visited[nr][nc])
        continue;

      // Carve passage between current and next cell.
      grid.removeWall(r, c, dir);
      visited[nr][nc] = true;
      cellStack.push({nr, nc});
      moved = true;
      break;
    }

    if (!moved)
      cellStack.pop();
  }
}

void Simulator::enqueueCommand(Command cmd) { commandQueue.push(cmd); }

void Simulator::step() {
  if (floodActive) {
    floodStep();
    return;
  }

  if (navigateActive) {
    navigateStep();
    return;
  }

  if (commandQueue.empty())
    return;

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

void Simulator::startFloodFill(int goalRow, int goalCol) {
  auto &costMap = grid.getMutableCostMap();

  int rows = grid.getHeight();
  int cols = grid.getWidth();

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      costMap[r][c] = 255;
    }
  }

  while (!floodQueue.empty()) {
    floodQueue.pop();
  }

  costMap[goalRow][goalCol] = 0;
  floodQueue.push({goalRow, goalCol});

  floodActive = true;
  std::cout << "Flood started\n";
}

void Simulator::floodStep() {
  if (floodQueue.empty()) {
    floodActive = false;
    return;
  }

  std::cout << "Flood step\n";

  auto current = floodQueue.front();
  floodQueue.pop();

  int row = current.first;
  int col = current.second;

  auto &costMap = grid.getMutableCostMap();
  const Cell &cell = grid.getCell(row, col);

  int currentCost = costMap[row][col];

  auto tryUpdate = [&](int nr, int nc) {
    if (nr < 0 || nc < 0 || nr >= grid.getHeight() || nc >= grid.getWidth())
      return;

    if (costMap[nr][nc] > currentCost + 1) {
      costMap[nr][nc] = currentCost + 1;
      floodQueue.push({nr, nc});
    }
  };

  // Check walls before expanding

  if (!cell.north)
    tryUpdate(row - 1, col);

  if (!cell.south)
    tryUpdate(row + 1, col);

  if (!cell.east)
    tryUpdate(row, col + 1);

  if (!cell.west)
    tryUpdate(row, col - 1);
}

void Simulator::navigateStep() {
  int row = mouse.getRow();
  int col = mouse.getCol();

  auto &costMap = grid.getMutableCostMap();
  const Cell &cell = grid.getCell(row, col);

  int currentCost = costMap[row][col];

  // If we reached goal
  if (currentCost == 0) {
    navigateActive = false;
    std::cout << "Reached goal\n";
    return;
  }

  int bestRow = row;
  int bestCol = col;
  Direction bestDir = mouse.getDirection();
  int bestCost = currentCost;

  auto tryNeighbor = [&](int nr, int nc, Direction dir) {
    if (nr < 0 || nc < 0 || nr >= grid.getHeight() || nc >= grid.getWidth())
      return;

    if (costMap[nr][nc] < bestCost) {
      bestCost = costMap[nr][nc];
      bestRow = nr;
      bestCol = nc;
      bestDir = dir;
    }
  };

  if (!cell.north)
    tryNeighbor(row - 1, col, Direction::NORTH);
  if (!cell.south)
    tryNeighbor(row + 1, col, Direction::SOUTH);
  if (!cell.east)
    tryNeighbor(row, col + 1, Direction::EAST);
  if (!cell.west)
    tryNeighbor(row, col - 1, Direction::WEST);

  // If no lower neighbor exists, stop
  if (bestCost >= currentCost) {
    navigateActive = false;
    std::cout << "No descent possible\n";
    return;
  }

  Direction currentDir = mouse.getDirection();

  if (currentDir != bestDir) {
    // Rotate toward target direction (simple version)
    int diff =
        (static_cast<int>(bestDir) - static_cast<int>(currentDir) + 4) % 4;

    if (diff == 1)
      mouse.turnRight();
    else if (diff == 3)
      mouse.turnLeft();
    else if (diff == 2) {
      mouse.turnRight();
      return; // next tick will turn again
    }
  } else {
    mouse.moveForward(grid);
  }
}