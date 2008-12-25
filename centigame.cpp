/*
  centigame.cpp
 
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

#include <QPainter>

#include <iostream>

#include <cmath>
#include <cstdlib>

#include "centigame.h"

#include "qjoystick.h"

#define PI (3.141592654)

double randDouble(double min, double max) {
  return (double(rand())/double(RAND_MAX))*(max-min)+min;
}

bool del_bullet(Bullet b) {
  return b.hit();
}

bool del_mush(Mushroom m) {
  return m.destroyed();
}
/*!
  Empty constructor just calls super class' constructor.
*/
CentiGame::CentiGame(QWidget *parent) : QWidget(parent),
					gameSpeed(20),
					step(5), max_speed(32),
					dx(0.0), dy(0.0),
					shooting(false),
					tillNextBullet(0),
					repaintAll(true) {

  
  winWidth = -1;
  winHeight = -1;
  images.push_back(QPixmap(tr(":/images/ship.svg")));
  SHIP_IMG = images.size()-1;
  clear_images.push_back(QPixmap(tr(":/images/ship_bg.svg")));
    
  images.push_back(QPixmap(tr(":/images/bullet.svg")));
  BULLET_IMG = images.size()-1;
  clear_images.push_back(QPixmap(tr(":/images/bullet_bg.svg")));
  
  images.push_back(QPixmap(tr(":/images/mushroom.svg")));
  MUSHROOM0_IMG = images.size()-1;
  clear_images.push_back(QPixmap(tr(":/images/mushroom_bg.svg")));
  
  images.push_back(QPixmap(tr(":/images/mushroom_hit1.svg")));
  MUSHROOM1_IMG = images.size()-1;
  clear_images.push_back(QPixmap(tr(":/images/mushroom_hit1_bg.svg")));
    
  images.push_back(QPixmap(tr(":/images/mushroom_hit2.svg")));
  MUSHROOM2_IMG = images.size()-1;
  clear_images.push_back(QPixmap(tr(":/images/mushroom_hit2_bg.svg")));
    
  images.push_back(QPixmap(tr(":/images/mushroom_hit3.svg")));
  MUSHROOM3_IMG = images.size()-1;
  clear_images.push_back(QPixmap(tr(":/images/mushroom_hit3_bg.svg")));
  
  images.push_back(QPixmap(tr(":/images/spider.svg")));
  SPIDER_IMG = images.size()-1;
  clear_images.push_back(QPixmap(tr(":/images/spider_bg.svg")));
  
  images.push_back(QPixmap(tr(":/images/scorpion.svg")));
  SCORPION_IMG = images.size()-1;
  clear_images.push_back(QPixmap(tr(":/images/scorpion_bg.svg")));
  
  images.push_back(QPixmap(tr(":/images/flea.svg")));
  FLEA_IMG = images.size()-1;
  clear_images.push_back(QPixmap(tr(":/images/flea_bg.svg")));
  
  images.push_back(QPixmap(tr(":/images/head_segment.svg")));
  HEAD_IMG = images.size()-1;
  clear_images.push_back(QPixmap(tr(":/images/head_segment_bg.svg")));
  
  images.push_back(QPixmap(tr(":/images/body_segment.svg")));
  BODY_IMG = images.size()-1;
  clear_images.push_back(QPixmap(tr(":/images/body_segment_bg.svg")));
  
  for (int i=0;i<100;++i) {
    shrooms.push_back(Mushroom(MUSHROOM0_IMG, MUSHROOM3_IMG,randDouble(0.0,31.0/32), randDouble(0.0,0.8)));
    needsUpdate.push_back(&shrooms[i]);
  }
  
  theShip = new PlayerShip(SHIP_IMG);

  bulletSound = snd.loadSound(tr("sound/laser.wav"));
  
  setFocusPolicy(Qt::StrongFocus);

  connect(&joyThread, SIGNAL(joyEvent(QJoystickEvent *)),
	  this, SLOT(joystickEvent(QJoystickEvent*)));

  connect(&joyThread, SIGNAL(error(QString)),
	  this, SLOT(errorHandler(QString)));
  
  joyThread.start();
  setAutoFillBackground(false);
  setAttribute(Qt::WA_OpaquePaintEvent);
  QPixmapCache::setCacheLimit(1024*40);

  timerId = startTimer(gameSpeed);
}

CentiGame::~CentiGame() {
  joyThread.stop();
  if (!joyThread.wait(50)) {
    joyThread.terminate();
    joyThread.wait();
    
  }
  if (theShip) {
    delete theShip;
  }
}

void CentiGame::timerEvent(QTimerEvent *event) {
  if (event->timerId() == timerId) {
    needsUpdate.clear();
    if (shooting && (tillNextBullet <=0) && (bullets.size()<20)) {
      tillNextBullet = 8;
      bullets.push_back(Bullet(BULLET_IMG, theShip->xpos()+(theShip->width()/2.0), theShip->ypos()));
      snd.playSound(bulletSound);
    }
    
    for (std::deque<Bullet>::iterator iter = bullets.begin();
	 iter != bullets.end();
	 ++iter) {
      iter->handleTimer();
      
      std::vector<Mushroom>::iterator sh;
      for (sh = shrooms.begin(); sh != shrooms.end(); ++sh) {
	if (sh->detectHit(*iter)) {
	  sh->takeHit();
	  iter->setHit();
	  if (!sh->destroyed()) {
	    needsUpdate.push_back(&(*sh));
	    clears.push_back(std::make_pair(QRect(sh->xpos()*winWidth-1, sh->ypos()*winHeight-1,
						  sh->width()*winWidth+2, sh->height()*winHeight+2),
					    sh->image()-1));

	  } else {
	    clears.push_back(std::make_pair(QRect(sh->xpos()*winWidth-1, sh->ypos()*winHeight-1,
						  sh->width()*winWidth+2, sh->height()*winHeight+2),
					    sh->image()));
	  }

	}
      }
      clears.push_back(std::make_pair(QRect(iter->old_xpos()*winWidth-1, iter->old_ypos()*winHeight-1,
					    iter->old_width()*winWidth+2, iter->old_height()*winHeight+2),
				      iter->image()));
	  

      if (!iter->hit()) {
	needsUpdate.push_back(&(*iter));
      }
    }
    
    std::deque<Bullet>::iterator  bd =
      std::remove_if(bullets.begin(), bullets.end(), del_bullet);
    
    if (bd!=bullets.end())
      bullets.erase(bd);
    
    std::vector<Mushroom>::iterator  md =
      std::remove_if(shrooms.begin(), shrooms.end(), del_mush);
    
    if (md!=shrooms.end())
      shrooms.erase(md);
    
    theShip->handleTimer();
    clears.push_back(std::make_pair(QRect(theShip->old_xpos()*winWidth,theShip->old_ypos()*winHeight,
					  theShip->old_width()*winWidth,theShip->old_height()*winHeight),
				    SHIP_IMG));
    needsUpdate.push_back(theShip);
    
    --tillNextBullet;
//     repaintAll = true;
    update();
  } else {
    QWidget::timerEvent(event);
  }
}


/*!
  Update the screen.
*/
void CentiGame::paintEvent(QPaintEvent *) {
  QPainter painter(this);

  if (repaintAll) {
    painter.fillRect(0,0,winWidth, winHeight, QColor(0,0,0,0xff));
    std::vector<Mushroom>::iterator sh;
    for (sh = shrooms.begin(); sh != shrooms.end(); ++sh) {
      painter.drawPixmap(QRect(sh->xpos()*winWidth,
			      sh->ypos()*winHeight,
			      sh->width()*winWidth,
			      sh->height()*winHeight),
			images[sh->image()]);
    }
    
    std::deque<Bullet>::iterator bt;
    for (bt = bullets.begin(); bt != bullets.end(); ++ bt) {
      painter.drawPixmap(QRect(bt->xpos()*winWidth, bt->ypos()*winHeight,
			      bt->width()*winWidth, bt->height()*winHeight),
			images[bt->image()]);
    }

    painter.drawPixmap(QRect(theShip->xpos()*winWidth,
			    theShip->ypos()*winHeight,
			    theShip->width()*winWidth,
			    theShip->height()*winHeight),
		      images[theShip->image()]);
    
    repaintAll = false;
  } else {

    std::vector<std::pair<QRect, int> >::iterator ci;
    for (ci = clears.begin();
	 ci != clears.end();
	 ++ci) {
      painter.drawPixmap(ci->first, clear_images[ci->second]);
    }

    std::vector<AnimatedObject*>::iterator nui;
    for (nui = needsUpdate.begin();
	 nui != needsUpdate.end();
	 ++nui) {
      AnimatedObject *ni = *nui;
      painter.drawPixmap(QRect(ni->xpos()*winWidth,
			      ni->ypos()*winHeight,
			      ni->width()*winWidth,
			      ni->height()*winHeight),
			images[ni->image()]);
    
    }
  }
  clears.clear();
  needsUpdate.clear();
}

/*!
  Handle key presses.
  Only the direction keys matter
*/
void CentiGame::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case (Qt::Key_Up):       theShip->setDy(-1.0); break;
  case (Qt::Key_Down):     theShip->setDy(1.0); break;
  case (Qt::Key_Right):    theShip->setDx(1.0); break;
  case (Qt::Key_Left):     theShip->setDx(-1.0); break;
  case (Qt::Key_Space):    shooting = true; break;
  default:                 QWidget::keyPressEvent(event);
  }
  //  update();
}

void CentiGame::keyReleaseEvent(QKeyEvent *event) {
  switch (event->key()) {
  case (Qt::Key_Up):       theShip->setDy(0.0); break;
  case (Qt::Key_Down):     theShip->setDy(0.0); break;
  case (Qt::Key_Right):    theShip->setDx(0.0); break;
  case (Qt::Key_Left):     theShip->setDx(0.0); break;
  case (Qt::Key_Space):    shooting = false; break;
  default:                 QWidget::keyPressEvent(event);
  }
  //  update();
}

/*!
  Handle key presses.
  Only the direction keys matter
*/
void CentiGame::joystickEvent(QJoystickEvent *event) {
  if (event->type() == QJoystickEvent::Axis) {
    int ax;
    double val;
    event->getAxisEvent(ax, val);

    if (ax == 0) {
      theShip->setDx(val);
    } else if (ax == 1) {
      theShip->setDy(val);
    }
  } else if (event->type() == QJoystickEvent::ButtonDown) {
    switch (event->button()) {
    case 1:
      shooting = true;
      break;
    }
  } else if (event->type() == QJoystickEvent::ButtonUp) {
    switch (event->button()) {
    case 1:
      shooting = false;
      break;
    }
  }

  event->accept();
  //  update();
}


/*!
  Resize the widget
  Makes sure the window is an even multiple of the block size.
*/
void CentiGame::resizeEvent(QResizeEvent *event) {
  winWidth = event->size().width();
  winHeight = event->size().height();
  repaintAll = true;
  update();
}

void CentiGame::errorHandler(QString errMsg) {
  QMessageBox::critical(this, tr("Error"),
			errMsg,
			QMessageBox::Ok,
			QMessageBox::NoButton,
			QMessageBox::NoButton);
}
