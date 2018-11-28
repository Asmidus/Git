#pragma once
#include "SDL_mixer.h"
#include <vector>

using namespace std;

class Audio {
public:
	Audio();
	~Audio();
	void playSound(int sound);
	void playMusic(int music);
	// Use this enum class to define the position for each sound in the vector of sounds
	enum sound {
		//playershoot,
		//playerhit,
		//enemyshoot,
		//enemyhit,
		//explosion
	};
	// Same as the sound enum
	enum music {
		background
	};
private:
	vector<Mix_Chunk*> sounds;
	vector<Mix_Music*> musics;
};
