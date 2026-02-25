#include "grid.h"
#include <iostream>

Grid::Grid(int w, int h) : width(w), height(h) {
  cells.resize(height, std::vector<Cell>(width));
  addOuterWalls();
  costMap.resize(height, std::vector<int>(width, 255));
}

void Grid::addOuterWalls() {
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {

      if (i == 0)
        cells[i][j].north = true;

      if (i == height - 1)
        cells[i][j].south = true;

      if (j == 0)
        cells[i][j].west = true;

      if (j == width - 1)
        cells[i][j].east = true;
    }
  }
}

void Grid::addWall(int row, int col, Direction dir) {
  if (dir == Direction::NORTH) {
    cells[row][col].north = true;
    if (row > 0)
      cells[row - 1][col].south = true;
  } else if (dir == Direction::EAST) {
    cells[row][col].east = true;
    if (col < width - 1)
      cells[row][col + 1].west = true;
  } else if (dir == Direction::SOUTH) {
    cells[row][col].south = true;
    if (row < height - 1)
      cells[row + 1][col].north = true;
  } else if (dir == Direction::WEST) {
    cells[row][col].west = true;
    if (col > 0)
      cells[row][col - 1].east = true;
  }
}

void Grid::removeWall(int row, int col, Direction dir) {
  if (dir == Direction::NORTH) {
    cells[row][col].north = false;
    if (row > 0)
      cells[row - 1][col].south = false;
  } else if (dir == Direction::EAST) {
    cells[row][col].east = false;
    if (col < width - 1)
      cells[row][col + 1].west = false;
  } else if (dir == Direction::SOUTH) {
    cells[row][col].south = false;
    if (row < height - 1)
      cells[row + 1][col].north = false;
  } else if (dir == Direction::WEST) {
    cells[row][col].west = false;
    if (col > 0)
      cells[row][col - 1].east = false;
  }
}

bool Grid::canMove(int row, int col, Direction dir) const {
  if (dir == Direction::NORTH)
    return !cells[row][col].north;
  else if (dir == Direction::EAST)
    return !cells[row][col].east;
  else if (dir == Direction::SOUTH)
    return !cells[row][col].south;
  else if (dir == Direction::WEST)
    return !cells[row][col].west;

  return false;
}

void Grid::print() const {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      std::cout << "[";
      std::cout << cells[i][j].north << cells[i][j].east << cells[i][j].south
                << cells[i][j].west;
      std::cout << "] ";
    }
    std::cout << std::endl;
  }
}

int Grid::getWidth() const noexcept { return width; }
int Grid::getHeight() const noexcept { return height; }
const Cell &Grid::getCell(int row, int col) const { return cells[row][col]; }

const std::vector<std::vector<int>> &Grid::getCostMap() const noexcept {
  return costMap;
}

std::vector<std::vector<int>> &Grid::getMutableCostMap() noexcept {
  return costMap;
}