#include "Audio.h"
#include <iostream>


Audio::Audio() {
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		return;
	}
	Mix_AllocateChannels(200);
	Mix_VolumeMusic(15);
	Mix_Volume(-1, 15);
	//sounds.push_back(Mix_LoadWAV("media/sounds/playershoot.wav"));
	//sounds.push_back(Mix_LoadWAV("media/sounds/playerhit.wav"));
	//sounds.push_back(Mix_LoadWAV("media/sounds/lowshoot.wav"));
	//sounds.push_back(Mix_LoadWAV("media/sounds/enemyhit.wav"));
	//sounds.push_back(Mix_LoadWAV("media/sounds/explosion.wav"));
	musics.push_back(Mix_LoadMUS("music/background.mp3"));
	for (int i = 0; i < musics.size();i++) {
		if (musics[i] == NULL) {
			std::cout << "Error loading music file #" << i << std::endl;
		}
	}
}


Audio::~Audio() {
	for (auto e : sounds) {
		delete e;
	}
	//for (auto m : musics) {
	//	delete m;
	//}
	Mix_CloseAudio();
}

void Audio::playSound(int sound) {
	Mix_PlayChannel(-1, sounds[sound], 0);
}

void Audio::playMusic(int music) {
	Mix_PlayMusic(musics[music], 0);
}