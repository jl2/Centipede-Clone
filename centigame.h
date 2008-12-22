/*
  centigame.h
 
  Copyright (C) 2008 Jeremiah LaRocco

  This file is part of Centipede

  Centipede is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Centipede is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Centipede.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HMWIDGET_H
#define HMWIDGET_H_

#include <QtGui>
#include <QWidget>

#include <deque>
#include <vector>
#include <utility>
#include <algorithm>

#include "qjoylistener.h"
#include "qsdlsound.h"

struct ship_t {
  ship_t() {};
  QImage *pic;
  int x, y;
  double theta;
  double vel;
  int width, height;
};

struct bullet_t {
  bullet_t() {};
bullet_t(int xp, int yp) : x(xp), y(yp) {};

  int x, y;
  
  static QImage *pic;
  static int width, height;
  static QSound *snd;
};

class CentiGame : public QWidget {
  Q_OBJECT;

 public:
  CentiGame(QWidget *parent = 0);
  ~CentiGame();

  
 protected:
  void paintEvent(QPaintEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
  void resizeEvent(QResizeEvent *event);
  void timerEvent(QTimerEvent *event);
  
 public slots:
  void joystickEvent(QJoystickEvent *event);
  void errorHandler(QString errMsg);

 private:
  ship_t *theShip;

  QJoyListener joyThread;
  QSdlSound snd;
  
  int winWidth;
  int winHeight;

  int gameSpeed;
  int timerId;

  double step;
  double max_speed;
  
  double dx;
  double dy;
  
  double xVel;
  double yVel;

  bool shooting;
  int tillNextBullet;
  std::deque<bullet_t> bullets;

  int bulletSound;
};

#endif
