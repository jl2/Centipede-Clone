/*
  destructable.h
 
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

#ifndef DESTRUCTABLE_H
#define DESTRUCTABLE_H

#include "bullet.h"

class Destructable {
 public:
 Destructable() : dest(false) {}
  
  virtual bool takeHit() = 0;

  virtual bool detectHit(Bullet b) = 0;

  bool destroyed() { return dest; }
  
 protected:
  bool dest;
};

#endif
