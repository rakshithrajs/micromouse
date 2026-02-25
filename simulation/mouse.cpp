#include "mouse.h"
#include <iostream>

Mouse::Mouse(int startRow, int startCol)
    : row(startRow), col(startCol), dir(Direction::NORTH) {}

void Mouse::moveForward(const Grid &grid) {
  if (!grid.canMove(row, col, dir)) {
    std::cout << "Wall ahead!\n";
    return;
  }

  if (dir == Direction::NORTH)
    row--;
  else if (dir == Direction::EAST)
    col++;
  else if (dir == Direction::SOUTH)
    row++;
  else if (dir == Direction::WEST)
    col--;
}

void Mouse::turnLeft() {
  dir = static_cast<Direction>((static_cast<int>(dir) + 3) % 4);
}

void Mouse::turnRight() {
  dir = static_cast<Direction>((static_cast<int>(dir) + 1) % 4);
}

void Mouse::printPosition() const {
  std::cout << "Mouse at (" << row << ", " << col << ")\n";
}

int Mouse::getRow() const noexcept { return row; }
int Mouse::getCol() const noexcept { return col; }
Direction Mouse::getDirection() const noexcept { return dir; }