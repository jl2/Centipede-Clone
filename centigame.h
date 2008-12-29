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

#include <set>
#include <deque>
#include <vector>
#include <utility>
#include <algorithm>

#include "qjoylistener.h"
#include "qsdlsound.h"

#include "playership.h"
#include "mushroom.h"
#include "bullet.h"
#include "spider.h"

class CentiGame : public QWidget {
  Q_OBJECT;

 public:
  CentiGame(QWidget *parent = 0);
  ~CentiGame();

  void newGame();
  bool isMute();
  void setMute(bool m);
  bool isPaused();
  void setPaused(bool p);
  int score() { return theScore; }
  
 signals:
  void scoreChanged(int sc);
  void levelChanged(int lv);
  void livesChanged(int lives);
  void gameLost();

  
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
  void erase(AnimatedObject *obj);
  void erase(Mushroom *obj);
  void draw(AnimatedObject *obj);
  
  PlayerShip *theShip;

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
  
  std::deque<Bullet> bullets;

  std::vector<Mushroom> shrooms;

  std::vector<AnimatedObject*> needsUpdate;
  std::vector< std::pair<QRect, int> > clears;
  std::vector<QPixmap> images;
  std::vector<QPixmap> clear_images;

  Spider *spid;

  
  int bulletSound;
  int shroomSound;
  int spiderSound;

  int SHIP_IMG;
  int BULLET_IMG;
  int MUSHROOM0_IMG;
  int MUSHROOM1_IMG;
  int MUSHROOM2_IMG;
  int MUSHROOM3_IMG;
  int SPIDER_IMG;
  int SCORPION_IMG;
  int FLEA_IMG;
  int HEAD_IMG;
  int BODY_IMG;
  bool repaintAll;

  int curLevel;
  int theScore;
  int remainingLives;
  int spiderFreq;
  int scorpionFreq;
  int fleaFreq;
  bool paused;
};

#endif
