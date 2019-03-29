#include "MainScene.h"

MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

MainScene* MainScene::create()
{
	MainScene* ret = nullptr;

	do
	{
		auto object = new MainScene();
		UTILS_BREAK_IF(object == nullptr);

		if (object->init())
		{
			object->autorelease();
		}
		else
		{
			delete object;
		}

		ret = object;
	} while (0);

	// return the object
	return ret;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
	bool ret = false;

	do
	{
		//////////////////////////////
		// 1. super init first
		UTILS_BREAK_IF(!parent::init());

		ret = true;

	} while (0);

	return ret;
}

Scene* MainScene::scene()
{
	Scene* result = nullptr;

	do
	{
		// 'layer' is an autorelease object
		auto layer = MainScene::create();
		UTILS_BREAK_IF(layer == nullptr);

		// 'scene' is an autorelease object
		auto scene = parent::createScene(layer);
		UTILS_BREAK_IF(scene == nullptr);

		result = scene;
	} while (0);

	// return the scene
	return result;
}
