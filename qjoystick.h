/*
  qjoystick.h
 
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

#ifndef Q_JOYSTICK_H
#define Q_JOYSTICK_H

#include <QEvent>

#include "SDL/SDL.h"


class QJoystickEvent : public QEvent {
 public:
  QJoystickEvent(SDL_Event *sev);
  
  int type();
  void getAxisEvent(int &which, double &newVal);
  int button();
  
  static const int Axis = 0;
  static const int ButtonDown = 1;
  static const int ButtonUp = 2;
  
 private:
  int event_type;
  int theAxis;
  double val;
  int wbutton;
};

/* const int QJoystickEvent::Axis; */
/* const int QJoystickEvent::Button; */

#endif
