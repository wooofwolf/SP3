#include "SoundController.h"

SoundController::SoundController()
{
}

SoundController::~SoundController()
{
}

void SoundController::Init()
{
	soundEngine = irrklang::createIrrKlangDevice();
}

void SoundController::PlaySoundFile(std::string fileName, bool loop)
{
	soundEngine->play2D(fileName.c_str(), loop);
}

void SoundController::StopSoundFile(std::string fileName)
{
	soundEngine->removeSoundSource(fileName.c_str());
}

void SoundController::StopAllSounds()
{
	soundEngine->stopAllSounds();
}

void SoundController::Exit()
{
	soundEngine->drop();
}
