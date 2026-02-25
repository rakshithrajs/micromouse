#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QKeyEvent>
#include <QPaintEvent>
#include <QPainterPath>
#include <QTimer>
#include <QWidget>

#include "../simulation/simulator.h"
#include <memory>

class MainWindow : public QWidget {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);

protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

private:
  std::unique_ptr<Simulator> simulator;
  QTimer *timer;
  bool isRunning = true;
};

#endif