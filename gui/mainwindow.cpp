#include "mainwindow.h"
#include <QKeyEvent>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), simulator(std::make_unique<Simulator>(16, 16)) {
  setFixedSize(600, 600);
  setFocusPolicy(Qt::StrongFocus);
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, [this]() {
    simulator->step();
    update();
  });

  timer->start(100);
}

void MainWindow::paintEvent(QPaintEvent *) {
  QPainter painter(this);

  const Grid &grid = simulator->getGrid();
  const Mouse &mouse = simulator->getMouse();

  int rows = grid.getHeight();
  int cols = grid.getWidth();
  int cellSize = width() / cols;

  const auto &costMap = grid.getCostMap();

  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {

      int cost = costMap[r][c];
      int shade = std::min(cost * 3, 255);

      painter.fillRect(c * cellSize, r * cellSize, cellSize, cellSize,
                       QColor(255 - shade, 255 - shade, 255));
    }
  }

  int cx = cols * cellSize + cellSize / 2;
  int cy = rows * cellSize + cellSize / 2;

  // Draw walls properly
  painter.setPen(QPen(Qt::black, 2));

  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {

      const Cell &cell = grid.getCell(r, c);

      int x = c * cellSize;
      int y = r * cellSize;

      if (cell.north)
        painter.drawLine(x, y, x + cellSize, y);

      if (cell.south)
        painter.drawLine(x, y + cellSize, x + cellSize, y + cellSize);

      if (cell.west)
        painter.drawLine(x, y, x, y + cellSize);

      if (cell.east)
        painter.drawLine(x + cellSize, y, x + cellSize, y + cellSize);
    }
  }

  painter.setBrush(Qt::red);
  int row = mouse.getRow();
  int col = mouse.getCol();
  int x = col * cellSize;
  int y = row * cellSize;

  // Draw a more detailed mouse shape
  QPainterPath mousePath;
  Direction dir = mouse.getDirection();

  // Create a rounded rectangle for the body
  mousePath.addRoundedRect(x + cellSize / 4, y + cellSize / 4, cellSize / 2,
                           cellSize / 2, cellSize / 8, cellSize / 8);

  // Add ears
  mousePath.addEllipse(x + cellSize / 4 - cellSize / 8,
                       y + cellSize / 4 - cellSize / 8, cellSize / 4,
                       cellSize / 4);
  mousePath.addEllipse(x + cellSize / 4 + cellSize / 2 - cellSize / 8,
                       y + cellSize / 4 - cellSize / 8, cellSize / 4,
                       cellSize / 4);

  // Draw the mouse body
  painter.fillPath(mousePath, Qt::red);

  // Draw an arrow indicating direction
  QPolygon arrow;
  if (dir == Direction::NORTH) {
    arrow << QPoint(x + cellSize / 2, y + cellSize / 8)
          << QPoint(x + cellSize / 4, y + cellSize / 4)
          << QPoint(x + 3 * cellSize / 4, y + cellSize / 4);
  } else if (dir == Direction::EAST) {
    arrow << QPoint(x + 3 * cellSize / 4, y + cellSize / 2)
          << QPoint(x + 3 * cellSize / 4, y + cellSize / 4)
          << QPoint(x + 3 * cellSize / 4, y + 3 * cellSize / 4);
  } else if (dir == Direction::SOUTH) {
    arrow << QPoint(x + cellSize / 2, y + 3 * cellSize / 4)
          << QPoint(x + cellSize / 4, y + 3 * cellSize / 4)
          << QPoint(x + 3 * cellSize / 4, y + 3 * cellSize / 4);
  } else if (dir == Direction::WEST) {
    arrow << QPoint(x + cellSize / 4, y + cellSize / 2)
          << QPoint(x + cellSize / 4, y + cellSize / 4)
          << QPoint(x + cellSize / 4, y + 3 * cellSize / 4);
  }

  painter.setBrush(Qt::black);
  painter.drawPolygon(arrow);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {

  Mouse &mouse = simulator->getMouse();
  Grid &grid = simulator->getGrid();

  switch (event->key()) {
  case Qt::Key_Up:
    simulator->enqueueCommand(Command::MoveForward);
    break;
  case Qt::Key_Left:
    simulator->enqueueCommand(Command::TurnLeft);
    break;
  case Qt::Key_Right:
    simulator->enqueueCommand(Command::TurnRight);
    break;
  case Qt::Key_P:
    if (isRunning) {
      timer->stop();
      isRunning = false;
    } else {
      timer->start(100);
      isRunning = true;
    }
    break;
  case Qt::Key_Space:
    if (!isRunning) {
      simulator->step();
      update();
    }
    break;
  case Qt::Key_F:
    simulator->startFloodFill(7, 7);
    break;
  case Qt::Key_G:
    simulator->startNavigation();
    break;
  }

  update();
}