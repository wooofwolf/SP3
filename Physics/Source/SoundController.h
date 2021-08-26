#pragma once

#include <string>

#include "SingletonPattern.h"
#include "../includes/irrKlang.h"

class SoundController : public SingletonPattern<SoundController>
{
	friend SingletonPattern<SoundController>;
protected:
	// Constructor
	SoundController();
	// Destructor
	virtual ~SoundController();

	irrklang::ISoundEngine* soundEngine;

public:
	void Init();
	void PlaySoundFile(std::string fileName, bool loop = false);
	void StopSoundFile(std::string fileName);
	void StopAllSounds();
	void Exit();
};

