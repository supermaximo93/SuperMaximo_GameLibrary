//============================================================================
// Name        : Music.cpp
// Author      : Max Foster
// Created on  : 30 May 2011
// Version     : 1.0
// Copyright   : http://creativecommons.org/licenses/by/3.0/
// Description : SuperMaximo GameLibrary Music class
//============================================================================

#include <iostream>
#include <vector>
using namespace std;
#include <SDL/SDL_mixer.h>
#include "../../headers/classes/Music.h"
#include "../../headers/Utils.h"
using namespace SuperMaximo;

namespace SuperMaximo {

Music::Music(const string & newName, const string & fileName) {
	name_ = newName, mixMusic = Mix_LoadMUS(fileName.c_str());
}

Music::~Music() {
	Mix_FreeMusic(mixMusic);
}

string Music::name() {
	return name_;
}

void Music::play() {
	Mix_PlayMusic(mixMusic, -1);
}

}
