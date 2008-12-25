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

#include "playership.h"

PlayerShip::PlayerShip(int img) : AnimatedObject(img,
					 0.5-(1.0/32), 0.85,
					 1.0/32,       1.0/24),
			   dx(0.0), dy(0.0),
			   shooting(false),
			   maxSpeed(1.0/32) { }

void PlayerShip::setDx(double x) {
  dx = x;
}

void PlayerShip::setDy(double y) {
  dy = y;

}

bool PlayerShip::handleTimer() {
  
  setXpos(xpos() + dx*maxSpeed);
  setYpos(ypos() + dy*maxSpeed);
  
  if (xpos() < 0.0)
    setXpos(0.0, false);
    
  if (xpos() > (1.0-width()))
    setXpos(1.0-width(),false);
      
  if (ypos() < 0.8)
    setYpos(0.8,false);
	
  if (ypos() > (1.0-height()))
    setYpos(1.0-height(),false);

  if (dx!= 0.0 || dy !=0.0)
    return true;

  return false;
}
