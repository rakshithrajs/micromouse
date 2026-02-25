#ifndef MOUSE_H
#define MOUSE_H

#include "grid.h"

class Mouse {
private:
  int row;
  int col;
  Direction dir;

public:
  Mouse(int startRow, int startCol);

  void moveForward(const Grid &grid);
  void turnLeft();
  void turnRight();

  void printPosition() const;
};

#endif