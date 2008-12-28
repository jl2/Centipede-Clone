/*
  mushroom.cpp
 
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

#include "mushroom.h"

Mushroom::Mushroom(int first_img, int last_img,
		   double x, double y) : AnimatedObject(first_img, x, y,
							1.0/32, 1.0/32,
							true),
					 Destructable(),
					 cur_img(first_img),
					 fimg(first_img),
					 limg(last_img) {}

bool Mushroom::takeHit() {
  ++cur_img;
  if (cur_img>limg) {
    dest = true;
  } else {
    setImage(cur_img);
    dest = false;
  }
  return dest;
}
  
bool Mushroom::detectHit(Bullet b) {
  return overlaps(&b);
}
bool Mushroom::reset() {
  dest = false;
    
  if (cur_img!=fimg) {
    setImage(fimg);
    cur_img = fimg;
    return true;
  }
  return false;
}
