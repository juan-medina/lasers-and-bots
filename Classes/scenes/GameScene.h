/****************************************************************************
 Copyright (c) 2019 Juan Medina

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
#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "../utils/physics/PhysicsTiledScene.h"

// forward declarations
class GameObject;
class RobotObject;
class SwitchObject;
class DoorObject;
class GameUi;
class RobotFragment;
class HarmObject;
class LevelManager;
class BackgroundLayer;
class CustomDrawNode;

class GameScene final : public PhysicsTiledScene
{
public:
  using BaseClass = PhysicsTiledScene;

  GameScene() noexcept;

  static Scene* scene(BasicApp* application, bool debugGrid, bool debugPhysics, int level);

  static GameScene* create(BasicApp* application, bool debugGrid, bool debugPhysics, int level);

  bool init(BasicApp* application, bool debugGrid, bool debugPhysics, unsigned short level);

  bool createGameUi();

  void calculateCameraBounds();

  bool createPhysicsContactsCallback();

  void setMapBoundsContactsSettings() const;

  void preLoadSounds() const;

  void getLevelSettings();

  static void cacheObjectsTextures();

  enum class Categories : unsigned short int
  {
    world = 1u,
    robot = 2u,
    spikes = 4u,
    saw = 8u,
    acid = 16u,
    door = 32u,
    switches = 64u,
    blocks = 128u,
    barrel = 256u,
    box = 512u,
    feet = 1024u,
    harm = spikes | acid | saw,
    walk_on = harm | blocks | barrel | box
  };

  void pause() override;

  void resume() override;

  void togglePause();

  void reload();

  void continueButton();

  // enter scene
  void onEnter() override;

  void updateUiPosition(const Vec2& finalPos) const;

  void updateBackgroundPosition(const Vec2& finalPos) const;

  void close();

  bool isPaused() const noexcept { return _paused; }

  void willEnterForeground() override;

private:
  // provide a physics node for a titled gid
  Node* providePhysicsNode(int gid) override;

  void updateGameTime(float delta);

  bool updateRobotShieldAndCheckIfDepleted() const;

  void checkRobotMovement(float delta);

  bool constexpr doWeNeedGameUpdates() const { return !(_paused || _doingFinalAnim); }

  void update(float delta) override;

  // move the camera following the robot clamping on the map
  void cameraFollowRobot(const Vec2& robotPosition, float delta);

  static void switchActivateDoor(DoorObject* door);
  void switchActivateSwitch(SwitchObject* switchObject);
  void switchActivateTarget(GameObject* target);

  bool isSwitchTargetingASwitch(SwitchObject* switchObject);

  void robotTouchSwitch(SwitchObject* switchObject);
  void robotTouchDoor(DoorObject* doorGameObject);
  void robotTouchHarmObjectStart(HarmObject* harmObject) const;
  void robotTouchHarmObjectEnd(HarmObject* harmObject) const;
  void robotTouchObjectStart(const PhysicsContact& contact);
  void robotTouchObjectEnd(const PhysicsContact& contact) const;

  void feetTouchObjectStart(const PhysicsContact& contact) const;
  void feetTouchObjectEnd(const PhysicsContact& contact) const;

  // two shapes start to contact
  bool onContactBegin(PhysicsContact& contact);

  // two shapes stop to contact
  void onContactSeparate(PhysicsContact& contact) const;

  // get an game object from a contact
  template <class Type>
  static Type* getObjectFromContact(const PhysicsContact& contact, Categories category);

  bool addLaser(const ValueMap& values, Node* layer);
  bool addRobot(const ValueMap& values, Node* layer);
  bool addSwitch(const ValueMap& values, Node* layer);
  bool addDoor(const ValueMap& values, Node* layer);
  bool addBarrel(const ValueMap& values, Node* layer);
  bool addSaw(const ValueMap& values, Node* layer);
  bool addBox(const ValueMap& values, Node* layer);
  bool addLight(const ValueMap& values) const;

  bool addObject(const vector<Value>::value_type& object);
  bool addObjectsToGame();

  static constexpr float GRAVITY = -5000.0f;

  unsigned short int calculateStars() const;

  void moveFragmentsToRobot();

  bool cacheRobotExplosion();

  void explodeRobot();

  void gameOver(bool win);

  void delayStart();

  void setCountdownNumberInUi(Ref* sender, int value) const;

  void start();

  RobotObject* _robot;
  GameUi* _gameUi;
  std::map<std::string, GameObject*> _gameObjects;
  std::vector<RobotFragment*> _robotFragments;

  Vec2 _lastRobotPosition;
  Vec2 _lastCameraPosition;

  Vec2 _minCameraPos;
  Vec2 _maxCameraPos;

  bool _paused;
  bool _doingFinalAnim;
  bool _doingDelayStart;
  bool _closing;

  float _totalTime;
  unsigned int _timeLimit;
  std::string _levelName;

  unsigned short int _barrelCount;
  unsigned short int _level;

  LevelManager* _levelManager;
  std::string _musicFileName;
  BackgroundLayer* _background;
  CustomDrawNode* _lightsNode;

  static constexpr float LIGHT_DISTANCE = 1500.f;
};

#endif //__GAMESCENE_H__
