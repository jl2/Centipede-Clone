/*
  qsdlsound.cpp
 
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

#include "qsdlsound.h"

QSdlSound::QSdlSound() : sdlInitializedHere(false),
			 sndInitializedHere(false) {
  if (!SDL_WasInit(SDL_INIT_VIDEO)) {
    SDL_Init(SDL_INIT_VIDEO);
    sdlInitializedHere = true;
  }
  if (!SDL_WasInit(SDL_INIT_AUDIO)) {
    SDL_InitSubSystem(SDL_INIT_AUDIO);
    sndInitializedHere = true;
  }
  //Initialize SDL_mixer
  Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );

}

QSdlSound::~QSdlSound() {
  std::vector<Mix_Chunk *>::iterator iter;
  
  for (iter = sounds.begin();
       iter != sounds.end();
       ++iter) {
    Mix_FreeChunk(*iter);
  }
  sounds.clear();

  if (sndInitializedHere) {
    //Quit SDL_mixer
    Mix_CloseAudio();
  }

  if (sdlInitializedHere) {
    //Quit SDL
    SDL_Quit();
  }
}

void QSdlSound::playSound(int snd) {
  Mix_PlayChannel( -1, sounds[snd], 0 );
}

int QSdlSound::loadSound(QString fn) {
  Mix_Chunk *theChunk;
  theChunk = Mix_LoadWAV(fn.toStdString().c_str());
  sounds.push_back(theChunk);
  return (sounds.size()-1);
}
