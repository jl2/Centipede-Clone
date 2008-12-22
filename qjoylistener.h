/*
  qjoylistener.h
 
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

#ifndef Q_JOYLISTENER_H
#define Q_JOYLISTENER_H

#include <QThread>

#include "qjoystick.h"

class QJoyListener : public QThread {
  Q_OBJECT
  
 public:
  QJoyListener();

  void stop();

 signals:
  void joyEvent(QJoystickEvent *je);
  void error(QString errMsg);

 protected:
  void run();

  
 private:
  bool quit;
};

#endif
