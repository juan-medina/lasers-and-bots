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

#ifndef __LAUNCHER_SCENE_H__
#define __LAUNCHER_SCENE_H__

#include "../utils/base/scene/BasicScene.h"

class LauncherScene : public BasicScene
{
public:
	// parent
	typedef BasicScene parent;

	// create with size
	static LauncherScene* create();

	// create the scene
	static Scene* scene();

	bool init();

	void play(Ref* sender);

	void screenModeClick(Ref* sender);

	void resolutionClick(Ref* sender);

	void effectsMutedClick(Ref* sender);

	void musicMutedClick(Ref* sender);

private:
	void updateLabels();

	bool _fullScreen;
	Label* _screenModeLabel;

	int _screenWidth;
	int _screenHeight;
	Label* _resolutionLabel;

	bool _effectsMuted;
	Label* _effectsMutedLabel;

	bool _musicMuted;
	Label* _musicMutedLabel;

	void loadSettings();
	void saveSettings();
};

#endif // __LAUNCHER_SCENE_H__
