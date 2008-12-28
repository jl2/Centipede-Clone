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
#include <ctime>
#include <cstdlib>

#include "utils.h"

#include "centigame.h"

#include "qjoystick.h"

#define PI (3.141592654)

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
					spid(0),
					repaintAll(true),
					curLevel(1),
					score(0),
					remainingLives(3),
					spiderFreq(200),
					scorpionFreq(5000),
					fleaFreq(2000) {
  std::srand(std::time(0));
  winWidth = -1;
  winHeight = -1;
  
  images.push_back(QPixmap(tr(":/images/ship.svg")));
  clear_images.push_back(QPixmap(tr(":/images/ship_bg.svg")));
  SHIP_IMG = images.size()-1;

  images.push_back(QPixmap(tr(":/images/bullet.svg")));
  clear_images.push_back(QPixmap(tr(":/images/bullet_bg.svg")));
  BULLET_IMG = images.size()-1;
  
  images.push_back(QPixmap(tr(":/images/mushroom.svg")));
  clear_images.push_back(QPixmap(tr(":/images/mushroom_bg.svg")));
  MUSHROOM0_IMG = images.size()-1;
  
  images.push_back(QPixmap(tr(":/images/mushroom_hit1.svg")));
  clear_images.push_back(QPixmap(tr(":/images/mushroom_hit1_bg.svg")));
  MUSHROOM1_IMG = images.size()-1;
  
  images.push_back(QPixmap(tr(":/images/mushroom_hit2.svg")));
  clear_images.push_back(QPixmap(tr(":/images/mushroom_hit2_bg.svg")));
  MUSHROOM2_IMG = images.size()-1;
  
  images.push_back(QPixmap(tr(":/images/mushroom_hit3.svg")));
  clear_images.push_back(QPixmap(tr(":/images/mushroom_hit3_bg.svg")));
  MUSHROOM3_IMG = images.size()-1;
  
  images.push_back(QPixmap(tr(":/images/spider.svg")));
  clear_images.push_back(QPixmap(tr(":/images/spider_bg.svg")));
  SPIDER_IMG = images.size()-1;
  
  images.push_back(QPixmap(tr(":/images/scorpion.svg")));
  clear_images.push_back(QPixmap(tr(":/images/scorpion_bg.svg")));
  SCORPION_IMG = images.size()-1;
  
  images.push_back(QPixmap(tr(":/images/flea.svg")));
  clear_images.push_back(QPixmap(tr(":/images/flea_bg.svg")));
  FLEA_IMG = images.size()-1;
  
  images.push_back(QPixmap(tr(":/images/head_segment.svg")));
  clear_images.push_back(QPixmap(tr(":/images/head_segment_bg.svg")));
  HEAD_IMG = images.size()-1;

  images.push_back(QPixmap(tr(":/images/body_segment.svg")));
  clear_images.push_back(QPixmap(tr(":/images/body_segment_bg.svg")));
  BODY_IMG = images.size()-1;

  for (int i=0;i<100;++i) {
    shrooms.push_back(Mushroom(MUSHROOM0_IMG, MUSHROOM3_IMG,randDouble(0.0,31.0/32), randDouble(0.0,0.8)));
    needsUpdate.push_back(&shrooms[i]);
  }
  
  theShip = new PlayerShip(SHIP_IMG);

  bulletSound = snd.loadSound(tr("sound/laser.wav"));
  shroomSound = snd.loadSound(tr("sound/mushroom_gen.wav"));
  spiderSound = snd.loadSound(tr("sound/spider_kill.wav"));
  
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
	    draw(&*sh);
	    erase(&*sh);
	  } else {
	    erase(&*sh);
	    ++score;
	    emit scoreChanged(score);
	  }

	}
      }
      erase(&*iter);
      
      if (spid) {
	if (spid->detectHit(*iter)) {
	  score += 100;
	  emit scoreChanged(score);
	  
	  erase(spid);
	  clears.push_back(std::make_pair(QRect(spid->xpos()*winWidth,spid->ypos()*winHeight,
					  spid->width()*winWidth,spid->height()*winHeight),
				  spid->image()));
	  snd.playSound(spiderSound);
	  iter->setHit();
	  delete spid;
	  spid = 0;
	}
      }
      if (!iter->hit()) {
	draw(&*iter);
      }
    }
    std::vector<Mushroom>::iterator sh;
    for (sh = shrooms.begin(); sh != shrooms.end(); ++sh) {
      if (sh->overlaps(theShip)) {
	erase(&*sh);
	draw(&*sh);
      }
      if (spid) {
	if (sh->overlaps(spid)) {
	  erase(&*sh);
	  draw(&*sh);
	}
      }
    }
      
    if (spid==0 && (std::rand()%spiderFreq == 0)) {
      spid = new Spider(SPIDER_IMG, curLevel*0.0025);
    }

    if (spid && spid->overlaps(theShip)) {
      killTimer(timerId);
      --remainingLives;
      emit livesChanged(remainingLives);

      erase(theShip);
      repaintAll = true;
      theShip->setXpos(0.5);
      delete spid;
      spid = 0;
      std::vector<Mushroom>::iterator sh;
      for (sh = shrooms.begin(); sh != shrooms.end(); ++sh) {
	  
	if (sh->reset()) {
	  erase(&*sh);
	  draw(&*sh);
	  snd.playSound(shroomSound);
	  usleep(750);
	}
      }
      if (remainingLives < 0) {
	update();
	bullets.clear();
	emit gameLost();
	return;
      }

      timerId = startTimer(gameSpeed);
    }
    
    if (spid) {
      spid->handleTimer();
      if (spid->xpos()<0.0 || spid->xpos()>1.0) {
	erase(spid);
	delete spid;
	spid = 0;
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
    erase(theShip);
    draw(theShip);
    
    if (spid) {
      erase(spid);
      draw(spid);
    }
    
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

void CentiGame::erase(AnimatedObject *obj) {
  clears.push_back(std::make_pair(QRect(obj->old_xpos()*winWidth,obj->old_ypos()*winHeight,
					  obj->old_width()*winWidth,obj->old_height()*winHeight),
				  obj->image()));
}

void CentiGame::erase(Mushroom *obj) {
  clears.push_back(std::make_pair(QRect(obj->old_xpos()*winWidth,obj->old_ypos()*winHeight,
					  obj->old_width()*winWidth,obj->old_height()*winHeight),
				  MUSHROOM0_IMG));
}

void CentiGame::draw(AnimatedObject *obj) {
  needsUpdate.push_back(obj);
}
