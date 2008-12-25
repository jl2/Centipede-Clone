/*
  animatedobject.h
 
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

#ifndef ANIMATED_OBJECT_H
#define ANIMATED_OBJECT_H

#include <iostream>

class AnimatedObject {
 public:
 AnimatedObject(int img,
	       double x, double y,
		double width, double height, bool fill=true) : pic(img),
    xp(x), yp(y), w(width), h(height), oxp(x), oyp(y), ow(w), oh(h), fbg(fill) { };
  

  int image() const { return pic; }
  double xpos() const   { return xp; }
  double ypos() const   { return yp; }
  double width() const  { return w; }
  double height() const { return h; }

  double old_xpos() const   { return oxp; }
  double old_ypos() const   { return oyp; }
  double old_width() const  { return ow; }
  double old_height() const { return oh; }

  void setXpos(double x, bool up = true) {
    if (up) 
      oxp = xp;

    xp = x;
  }
  void setYpos(double y, bool up = true) {
    if (up)
      oyp = yp;
    yp = y;
  }
  void setWidth(double wid, bool up = true) {
    if (up)
      ow = w;
    w = wid;
  }
  void setHeight(double hei, bool up = true) {
    if (up)
      oh = h;
    h = hei;
  }
  
  void setLocation(double x, double y, bool up = true) {
    if (up) {
      oxp = xp;
      oyp = yp;
    }
    xp = x;
    yp = y;
  }
    
  void setSize(double wid, double hei, bool up) {
    if (up) {
      ow = w;
      oh = h;
    }
    w = wid;
    h = hei;
  }
    
  void setImage(int img) { pic = img; }

  bool fillBg() { return fbg; }
    
 private:
  int pic;
  
  double xp, yp;
  double w, h;

  double oxp, oyp;
  double ow, oh;
  bool fbg;
};

#endif
