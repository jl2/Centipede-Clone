/*
  qjoylistener.cpp
 
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

#include "qjoylistener.h"

QJoyListener::QJoyListener() : QThread(),
			       quit(false) {
}

void QJoyListener::run() {
  SDL_Event event;
  SDL_Joystick *stick = NULL;
  bool sdlInitializedHere = false;
  bool joyInitializedHere = false;
  
  if (!SDL_WasInit(SDL_INIT_VIDEO)) {
    SDL_Init(SDL_INIT_VIDEO);
    sdlInitializedHere = true;
  }
  if (!SDL_WasInit(SDL_INIT_JOYSTICK)) {
    if( SDL_InitSubSystem( SDL_INIT_JOYSTICK ) == -1 ) {
      emit error(tr("Could not initialize SDL."));
      return;
    }
    joyInitializedHere = true;
  }
  
  //Check if there's any joysticks
  if( SDL_NumJoysticks() < 1 ) {
    emit error(tr("No joystick found."));
    return;
  }
    
  //Open the joystick
  stick = SDL_JoystickOpen( 0 );
    
  //If there's a problem opening the joystick
  if( stick == NULL ) {
    emit error(tr("Could not open joystick."));
    return;
  }
    
  while( quit == false ) {
    SDL_WaitEvent(&event);
    emit joyEvent(new QJoystickEvent(&event));
  }
  
  SDL_JoystickClose( stick );
  
  if (sdlInitializedHere) {
    SDL_Quit();
  }

}
void QJoyListener::stop() {
  quit = true;
}
