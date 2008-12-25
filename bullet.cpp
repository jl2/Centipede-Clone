/*
  bullet.cpp
 
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

#include "bullet.h"

Bullet::Bullet(int img, double x, double y) : AnimatedObject(img,
							    x-1.0/128, y,
							    1.0/64,       1.0/48),
						 dy((1.0/48)/2), h(false) { }

void Bullet::setDy(double y) {
  dy = y;
}

bool Bullet::handleTimer() {
  
  setYpos(ypos() - dy);

  if (ypos()<0) {
    h = true;
    return true;
  }
  if (ypos() > (1.0-height())) {
    setYpos(1.0-height());
    h = true;
    return true;
  }

  return false;
}

bool Bullet::hit() {
  return h;
}

void Bullet::setHit() {
  h = true;
}
