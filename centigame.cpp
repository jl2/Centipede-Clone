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

#include "centigame.h"

#include "qjoystick.h"

#define PI (3.141592654)

extern int bullet_t::height;
extern int bullet_t::width;
extern QImage* bullet_t::pic;
extern QSound* bullet_t::snd;

bool del_bullet(bullet_t b) {
  return (b.y<0);
}

/*!
  Empty constructor just calls super class' constructor.
*/
CentiGame::CentiGame(QWidget *parent) : QWidget(parent),
					gameSpeed(20),
					step(5), max_speed(32),
					dx(0.0), dy(0.0),
					shooting(false),
					tillNextBullet(0) {
  theShip = new ship_t();
  theShip->x = 50;
  theShip->y = 50;
  theShip->width = 64;
  theShip->height = 64;
  theShip->vel = 0.0;
  theShip->theta = 90.0;
  theShip->pic = new QImage(tr(":/images/ship.svg"));

  bullet_t::width = 16;
  bullet_t::height = 16;
  bullet_t::pic = new QImage(tr(":/images/bullet.svg"));
  
//   if (QSound::isAvailable()) {
//     bullet_t::snd = new QSound(tr("./sound/laser.wav"));
//     bullet_t::snd->play();
//   } else {
//     std::cout << "Sound not available!\n";
//   }
  bulletSound = snd.loadSound(tr("sound/laser.wav"));
  
  setFocusPolicy(Qt::StrongFocus);

  connect(&joyThread, SIGNAL(joyEvent(QJoystickEvent *)),
	  this, SLOT(joystickEvent(QJoystickEvent*)));

  connect(&joyThread, SIGNAL(error(QString)),
	  this, SLOT(errorHandler(QString)));
  
  joyThread.start();
  timerId = startTimer(gameSpeed);
}

CentiGame::~CentiGame() {
  joyThread.stop();
  if (!joyThread.wait(50)) {
    joyThread.terminate();
    joyThread.wait();
    
  }
  if (theShip) {
    delete theShip->pic;
    delete theShip;
  }
}

void CentiGame::timerEvent(QTimerEvent *event) {
  if (event->timerId() == timerId) {

    int mw = theShip->width;
    int mh = theShip->height;
    
    theShip->x += dx*max_speed;
    theShip->y += dy*max_speed;

    if (theShip->x < 0)
      theShip->x = 0;
    
    if (theShip->x > (winWidth-mw))
	theShip->x = (winWidth-mw);
      
    if (theShip->y < (winHeight*0.85))
      theShip->y = (winHeight*0.85);
	
    if (theShip->y > (winHeight-mh))
      theShip->y = winHeight-mh;

    if (shooting && (tillNextBullet <=0) && (bullets.size()<20)) {
      tillNextBullet = 8;
      bullets.push_back(bullet_t(theShip->x+(theShip->width/2)-bullet_t::width/2, theShip->y-bullet_t::height/2));
      snd.playSound(bulletSound);
//       if (bullet_t::snd)
// 	bullet_t::snd->play();
    }
    
    for (std::deque<bullet_t>::iterator iter = bullets.begin();
	 iter != bullets.end();
	 ++iter) {
      iter->y -= bullet_t::height/3;
    }
    std::deque<bullet_t>::iterator  ri =
      std::remove_if(bullets.begin(), bullets.end(), del_bullet);
    if (ri!=bullets.end())
      bullets.erase(ri);
    
    --tillNextBullet;
//     std::cout << "X: " << theShip->x << "\nY: " << theShip->y << "\n";
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
  painter.fillRect(0, 0,
		   size().width(),
		   size().height(),
		   QColor(0,0,0,0xff));

//   painter.fillRect(0, 0,
// 		   size().width(),
// 		   size().height()*0.85,
// 		   QColor(0x33,0x33,0x33,0xff));

  painter.drawImage(theShip->x,
		    theShip->y,
		    theShip->pic->scaled(theShip->width,
					 theShip->height));

  for (std::deque<bullet_t>::iterator iter = bullets.begin();
       iter != bullets.end();
       ++iter) {
    painter.drawImage(iter->x,
		      iter->y,
		      bullet_t::pic->scaled(bullet_t::width,
					    bullet_t::height));
  }

}

/*!
  Handle key presses.
  Only the direction keys matter
*/
void CentiGame::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case (Qt::Key_Up):       dy = -1.0; break;
  case (Qt::Key_Down):     dy = 1.0; break;
  case (Qt::Key_Right):    dx = 1.0; break;
  case (Qt::Key_Left):     dx = -1.0; break;
  case (Qt::Key_Space):    shooting = true; break;
  default:                 QWidget::keyPressEvent(event);
  }
  //  update();
}

void CentiGame::keyReleaseEvent(QKeyEvent *event) {
  switch (event->key()) {
  case (Qt::Key_Up):       dy = 0.0; break;
  case (Qt::Key_Down):     dy = 0.0; break;
  case (Qt::Key_Right):    dx = 0.0; break;
  case (Qt::Key_Left):     dx = 0.0; break;
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
      dx = val;
    } else if (ax == 1) {
      dy = val;
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
  max_speed = (winWidth/20);
  theShip->width = winWidth/16;
  theShip->height = winHeight/16;
  bullet_t::width = winWidth/32;
  bullet_t::height = winHeight/32;
}

void CentiGame::errorHandler(QString errMsg) {
  QMessageBox::critical(this, tr("Error"),
			errMsg,
			QMessageBox::Ok,
			QMessageBox::NoButton,
			QMessageBox::NoButton);
}
