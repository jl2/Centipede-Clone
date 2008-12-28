/*
  spider.cpp
 
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

#include <cmath>
#include <cstdlib>
#include "spider.h"
#include "utils.h"


Spider::Spider(int img, double speed) : AnimatedObject(img,
						       std::rand()%2,
						       randDouble(0.8, 1.0),
						       1.0/32, 1.0/32, true),
					TimedObject(),
					Destructable(),
					spd(std::fabs(speed)),
					dx(std::fabs(speed)), dy(std::fabs(speed)) {
  dir = -1.0;
  if (xpos()==0)
    dir = 1.0;
  xgoal = xpos() + dir * randDouble(0.0,0.1);
  ygoal = ypos() + randDouble(-0.2, 0.2);
    
  if (ygoal<0.0) ygoal = 0.0;
  if (ygoal>1.0) ygoal = 1.0;
    
  if (ygoal<ypos()) dy = -dy;
  if (xgoal<xpos()) dx = -dx;
}

bool Spider::takeHit() {
  dest = true;
  return dest;
}
  
bool Spider::detectHit(Bullet b) {
  return overlaps(&b);
}
  
bool Spider::handleTimer() {
  setXpos(xpos() + dx);
  setYpos(ypos() + dy);
    
  if (dx<0) {
    if (xpos()<xgoal) {
      xgoal = xpos() + dir * randDouble(0.0,0.2);
      dx = spd;
      if (xpos()>xgoal) dx = -spd;
    }
  } else {
    if (xpos()>xgoal) {
      xgoal = xpos() + dir * randDouble(0.0,0.2);
      dx = spd;
      if (xpos()>xgoal) dx = -spd;
    }
  }
  if (dy<0) {
    if (ypos()<ygoal) {
      ygoal = ypos() + randDouble(-0.2, 0.2);
      if (ygoal<height()) ygoal = height();
      if (ygoal>1.0) ygoal = 1.0;
      dy = spd;
      if (ypos()>ygoal) dy = -spd;
    }
  } else {
    if (ypos()>ygoal) {
      ygoal = ypos() + randDouble(-0.2, 0.2);
      if (ygoal<height()) ygoal = height();
      if (ygoal>1.0) ygoal = 1.0;
      dy = spd;
      if (ypos()>ygoal) dy = -spd;
    }
  }

  if ((xpos()<0.0) || (xpos()>1.0))
    return true;

  return false;
}
