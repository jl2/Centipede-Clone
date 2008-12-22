/*
  qsdlsound.h
 
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

#ifndef Q_SDL_SOUND_H
#define Q_SDL_SOUND_H

#include <QString>

#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

class QSdlSound {
 public:
  QSdlSound();
  ~QSdlSound();
  
  void playSound(int snd);
  int loadSound(QString fn);


 private:

  std::vector<Mix_Chunk *> sounds;
  
  bool sdlInitializedHere;
  bool sndInitializedHere;
};

#endif
