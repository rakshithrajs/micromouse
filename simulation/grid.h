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
  std::vector<std::vector<int>> costMap;

public:
  Grid(int width, int height);
  void print() const;
  void addOuterWalls();

  bool canMove(int x, int y, Direction dir) const;
  void addWall(int x, int y, Direction dir);

  int getWidth() const noexcept;
  int getHeight() const noexcept;
  const Cell &getCell(int row, int col) const;
  const std::vector<std::vector<int>> &getCostMap() const noexcept;
};

#endif