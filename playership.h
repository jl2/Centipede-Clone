/*
  playership.h
 
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

#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H

#include "animatedobject.h"
#include "timedobject.h"

class PlayerShip : public AnimatedObject, public TimedObject {
 public:
  PlayerShip(int img);
  
  void setDx(double x);
  void setDy(double y);

  bool handleTimer();
  
 private:
  double dx;
  double dy;

  bool shooting;
  
  double maxSpeed;
};

#endif
