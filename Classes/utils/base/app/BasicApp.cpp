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

#include "BasicApp.h"
#include "../../audio/AudioHelper.h"

BasicApp::BasicApp(const char* applicationName)
{
	_name = applicationName;
}

BasicApp::~BasicApp()
{
	AudioHelper::getInstance()->appExit();
}

// if you want a different context,just modify the value of glContextAttrs
// it will takes effect on all platforms
void BasicApp::initGLContextAttrs()
{
	// set OpenGL context attributions,now can only set six attributions:
	// red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

	GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
	return 0; // flag for packages manager
}

bool BasicApp::applicationDidFinishLaunching()
{

	bool ret = false;

	do
	{
		// read settings

		auto designWidth = UserDefault::getInstance()->getIntegerForKey("designWidth", 1920);
		auto designHeight = UserDefault::getInstance()->getIntegerForKey("designHeight", 1080);

		UserDefault::getInstance()->setIntegerForKey("designWidth", designWidth);
		UserDefault::getInstance()->setIntegerForKey("designHeight", designHeight);

		auto screenWidth = UserDefault::getInstance()->getIntegerForKey("screenWidth", 1920);
		auto screenHeight = UserDefault::getInstance()->getIntegerForKey("screenHeight", 1080);

		UserDefault::getInstance()->setIntegerForKey("screenWidth", screenWidth);
		UserDefault::getInstance()->setIntegerForKey("screenHeight", screenHeight);

		_designResolution = Size(designWidth, designHeight);
		_screenSize = Size(screenWidth, screenHeight);

		auto fullScreen = UserDefault::getInstance()->getBoolForKey("fullScreen", false);
		UserDefault::getInstance()->setBoolForKey("fullScreen", fullScreen);

		auto fitAll = UserDefault::getInstance()->getBoolForKey("fitAll", false);
		UserDefault::getInstance()->setBoolForKey("fitAll", fitAll);

		// initialize director and view
		auto director = Director::getInstance();
		auto glview = director->getOpenGLView();

		if (!glview)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
			if (fullScreen)
			{
				glview = GLViewImpl::createWithFullScreen(_name);
			}
			else
			{
				glview = GLViewImpl::createWithRect(_name, Rect(0, 0, _screenSize.width, _screenSize.height));
			}
#else
			glview = GLViewImpl::create(_name);
#endif
			director->setOpenGLView(glview);
		}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		if (!fullScreen)
		{
			this->CenterWin32Window();
		}
#endif
		// show fps flag, by default false
		bool showFPS = false;

		// get store value for show fps
		showFPS = UserDefault::getInstance()->getBoolForKey("showFps", showFPS);

		// store the value
		UserDefault::getInstance()->setBoolForKey("showFps", showFPS);
		UserDefault::getInstance()->flush();

		// turn on display FPS
		director->setDisplayStats(showFPS);

		// set FPS. the default value is 1.0/60 if you don't call this
		director->setAnimationInterval(1.0f / 60.0f);

		// Set the design resolution, we scale to our design with so aspect ratio is mantained
		if (fitAll)
		{
			glview->setDesignResolutionSize(_designResolution.width, _designResolution.height, ResolutionPolicy::SHOW_ALL);
		}
		else
		{
			if (_designResolution.width > _designResolution.height)
			{
				glview->setDesignResolutionSize(_designResolution.width, _designResolution.height, ResolutionPolicy::FIXED_WIDTH);
			}
			else
			{
				glview->setDesignResolutionSize(_designResolution.width, _designResolution.height, ResolutionPolicy::FIXED_HEIGHT);
			}
		}

		// get the real screen scale
		Point realScreen = Vec2(glview->getScaleX(), glview->getScaleY());

		// for plugings
		register_all_packages();

		// create a scene. it's an autorelease object
		auto scene = this->initScene();
		UTILS_BREAK_IF(scene == nullptr);

		// run
		director->runWithScene(scene);

		ret = true;
	} while (0);

	return ret;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void BasicApp::applicationDidEnterBackground()
{
	Director::getInstance()->stopAnimation();

	AudioHelper::getInstance()->appToBG();
}

// this function will be called when the app is active again
void BasicApp::applicationWillEnterForeground()
{
	Director::getInstance()->startAnimation();

	AudioHelper::getInstance()->appToFG();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void BasicApp::CenterWin32Window()
{
	// get the window handler
	HWND hwndWindow = Director::getInstance()->getOpenGLView()->getWin32Window();

	if (!hwndWindow)
	{
		return;
	}

	// get the desktop dimensions
	RECT rcDesktop, rcWindow;
	GetWindowRect(GetDesktopWindow(), &rcDesktop);

	// substract the task bar
	HWND hTaskBar = FindWindow(TEXT("Shell_TrayWnd"), NULL);
	if (hTaskBar != NULL)
	{
		APPBARDATA abd;

		abd.cbSize = sizeof(APPBARDATA);
		abd.hWnd = hTaskBar;

		SHAppBarMessage(ABM_GETTASKBARPOS, &abd);
		SubtractRect(&rcDesktop, &rcDesktop, &abd.rc);
	}

	// get our window dimensions
	GetWindowRect(hwndWindow, &rcWindow);

	// calculate center position
	int offsetX = rcDesktop.left + (rcDesktop.right - rcDesktop.left - (rcWindow.right - rcWindow.left)) / 2;
	offsetX = (offsetX > 0) ? offsetX : rcDesktop.left;
	int offsetY = rcDesktop.top + (rcDesktop.bottom - rcDesktop.top - (rcWindow.bottom - rcWindow.top)) / 2;
	offsetY = (offsetY > 0) ? offsetY : rcDesktop.top;

	// move the window
	SetWindowPos(hwndWindow, 0, offsetX, offsetY, 0, 0, SWP_NOCOPYBITS | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}
#endif
