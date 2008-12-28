/*
  spider.h
 
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

#ifndef SPIDER_H
#define SPIDER_H
#include <cmath>

#include "utils.h"

#include "bullet.h"
#include "animatedobject.h"
#include "destructable.h"

class Spider : public AnimatedObject, public TimedObject, public Destructable {
 public:
  Spider(int img, double speed);
  bool takeHit();
  bool detectHit(Bullet b);
  bool handleTimer();
 private:
  double xgoal, ygoal;  // Where it's moving to
  double dir;
  double spd;
  double dx, dy;
};

#endif
