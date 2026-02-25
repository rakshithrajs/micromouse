#ifndef GRID_H
#define GRID_H

#include <vector>

enum class Direction { NORTH, EAST, SOUTH, WEST };

struct Cell {
  bool north;
  bool east;
  bool south;
  bool west;

  Cell() : north(false), east(false), south(false), west(false) {}
};

class Grid {
private:
  int width;
  int height;
  std::vector<std::vector<Cell>> cells;

public:
  Grid(int width, int height);
  void print() const;
  void addOuterWalls();

  bool canMove(int x, int y, Direction dir) const;
  void addWall(int x, int y, Direction dir);
};

#endif