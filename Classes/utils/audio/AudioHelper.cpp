/****************************************************************************
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AudioHelper.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

AudioHelper* AudioHelper::getInstance()
{
	// create if not create default heper and return it
	static AudioHelper helper;

	return &helper;
}

AudioHelper::AudioHelper()
{
	// not created yet
	_inited = false;

	// init object
	init();
}

AudioHelper::~AudioHelper()
{
	// if we are init we need to end
	if (_inited)
	{
		end();
	}
}

bool AudioHelper::init()
{
	// get configuration
	this->setEffectsMuted(UserDefault::getInstance()->getBoolForKey("effectsMuted", false));
	this->setMusicMuted(UserDefault::getInstance()->getBoolForKey("musicMuted", false));

	// not music played, yet
	_lastMusic = "";

	// object is init
	_inited = true;

	return true;
}

void AudioHelper::end()
{
	// end audio engine
	SimpleAudioEngine::end();

	// save config values
	this->saveValues();

	// we are not init
	_inited = false;
}

unsigned int AudioHelper::playEffect(const char* fileName)
{
	// if we are muted exit
	if (this->getEffectsMuted())
	{
		return 0;
	}
	else
	{
		// play effect
		return SimpleAudioEngine::getInstance()->playEffect(fileName);
	}
}

void AudioHelper::playMusic(const char* fileName)
{
	// store music name
	_lastMusic = std::string(fileName);

	// if we are muted exit
	if (this->getMusicMuted())
	{
		return;
	}
	else
	{
		// play music
		SimpleAudioEngine::getInstance()->playBackgroundMusic(_lastMusic.c_str(), true);
	}
}

void AudioHelper::preLoadEffect(const char* fileName)
{
	// preload effect
	SimpleAudioEngine::getInstance()->preloadEffect(fileName);
}

void AudioHelper::preLoadMusic(const char* fileName)
{
	// preload music
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(fileName);
}

void AudioHelper::toggleSound()
{
	// if we are muted
	if (this->getEffectsMuted())
	{
		// unmute
		this->setEffectsMuted(false);
	}

	else
	{
		// stop all effects and set to mute
		SimpleAudioEngine::getInstance()->stopAllEffects();
		this->setEffectsMuted(true);
	}

	// save config values
	this->saveValues();
}

void AudioHelper::toggleMusic()
{
	// if we are mute
	if (this->getMusicMuted())
	{
		// unmute
		this->setMusicMuted(false);

		// if we have last music
		if (_lastMusic.length())
		{
			// play again
			this->playMusic(_lastMusic.c_str());
		}
	}

	else
	{
		// stop music and mute
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		this->setMusicMuted(true);
	}

	// save config values
	this->saveValues();
}

void AudioHelper::appToBG()
{
	// if we wasn't muted pause music
	if (!this->getMusicMuted())
	{
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	}
}

void AudioHelper::appToFG()
{
	// if we wasn't muted resume music
	if (!this->getMusicMuted())
	{
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
}

void AudioHelper::saveValues()
{
	// save config vlaues
	UserDefault::getInstance()->setBoolForKey("effectsMuted", this->getEffectsMuted());
	UserDefault::getInstance()->setBoolForKey("musicMuted", this->getMusicMuted());

	UserDefault::getInstance()->flush();
}

void AudioHelper::appExit()
{
	this->end();
}

void AudioHelper::stopAllSounds()
{
	SimpleAudioEngine::getInstance()->stopAllEffects();
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void AudioHelper::unloadEffect(const char* file)
{
	SimpleAudioEngine::getInstance()->unloadEffect(file);
}
