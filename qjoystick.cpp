/*
  qjoystick.cpp
 
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

#include <iostream>

#include "qjoystick.h"

QJoystickEvent::QJoystickEvent(SDL_Event *sev) : QEvent(QEvent::User) {
  switch (sev->type) {
  case SDL_JOYAXISMOTION:
    if (sev->jaxis.which == 0) {
      event_type = Axis;
      theAxis = sev->jaxis.axis;
      val = double(sev->jaxis.value)/32768.0;
      wbutton = -1;
    }
    break;
  case SDL_JOYBUTTONDOWN:
    if (sev->jbutton.which == 0) {
      event_type = ButtonDown;
      wbutton = int(sev->jbutton.button);
      theAxis = -1;
      val = 0.0;
    }
    break;
  case SDL_JOYBUTTONUP:
    if (sev->jbutton.which == 0) {
      event_type = ButtonUp;
      wbutton = int(sev->jbutton.button);
      theAxis = -1;
      val = 0.0;
    }
  default:
    ; // Ignore
  }

}
  
int QJoystickEvent::type() {
  return event_type;
}

void QJoystickEvent::getAxisEvent(int &which, double &newVal) {
  which = theAxis;
  newVal = val;
}

int QJoystickEvent::button() {
  return wbutton;
}
