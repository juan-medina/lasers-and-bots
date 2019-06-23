#include "ResizableWindow.h"

ResizableWindow::ResizableWindow() : _labelTitle(nullptr) {}

ResizableWindow* ResizableWindow::create(const std::string& title, const float width, const float height,
                                         const Color3B& color /*= Color3B(0, 255, 255)*/)
{
  ResizableWindow* ret = nullptr;

  do
  {
    auto object = new ResizableWindow();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(title, width, height, color))
    {
      object->autorelease();
    }
    else
    {
      delete object;
      object = nullptr;
    }

    ret = object;
  } while (false);

  return ret;
}

bool ResizableWindow::init(const std::string& title, const float width, const float height,
                           const Color3B& color /*= Color3B(0, 255, 255)*/)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!BaseClass::init());

    const auto leftUp = Sprite::createWithSpriteFrameName("14_window_left_up.png");
    UTILS_BREAK_IF(leftUp == nullptr);
    addChild(leftUp);

    const auto rightUp = Sprite::createWithSpriteFrameName("14_window_right_up.png");
    UTILS_BREAK_IF(rightUp == nullptr);
    addChild(rightUp);

    const auto leftDown = Sprite::createWithSpriteFrameName("14_window_left_down.png");
    UTILS_BREAK_IF(leftDown == nullptr);
    addChild(leftDown);

    const auto rightDown = Sprite::createWithSpriteFrameName("14_window_right_down.png");
    UTILS_BREAK_IF(rightDown == nullptr);
    addChild(rightDown);

    const auto upInfo = Sprite::createWithSpriteFrameName("14_window_up.png");
    UTILS_BREAK_IF(upInfo == nullptr);
    addChild(upInfo);

    const auto leftInfo = Sprite::createWithSpriteFrameName("14_window_left.png");
    UTILS_BREAK_IF(leftInfo == nullptr);
    addChild(leftInfo);

    const auto interiorWidth = width - leftUp->getContentSize().width - rightUp->getContentSize().width;
    const auto horizontalSegments = static_cast<int>(interiorWidth / upInfo->getContentSize().width);
    const auto realWidth = (horizontalSegments * upInfo->getContentSize().width) +
                           leftUp->getContentSize().width + rightUp->getContentSize().width;

    const auto interiorHeight = height - leftUp->getContentSize().height - leftDown->getContentSize().height;
    const auto verticalSegments = static_cast<int>(interiorHeight / leftInfo->getContentSize().height);
    const auto realHeight = (verticalSegments * leftInfo->getContentSize().height) +
                            leftUp->getContentSize().height + leftDown->getContentSize().height;

    removeChild(upInfo);
    removeChild(leftInfo);

    const auto origin = Vec2(-realWidth / 2, realHeight / 2);
    const auto dest = origin + Vec2(realWidth, -realHeight);

    leftUp->setPosition(origin.x + leftUp->getContentSize().width / 2,
                        origin.y - leftUp->getContentSize().height / 2);

    rightUp->setPosition(dest.x - rightUp->getContentSize().width / 2,
                         origin.y - rightUp->getContentSize().height / 2);

    leftDown->setPosition(origin.x + leftDown->getContentSize().width / 2,
                          dest.y + leftDown->getContentSize().height / 2);

    rightDown->setPosition(dest.x - rightDown->getContentSize().width / 2,
                           dest.y + rightDown->getContentSize().height / 2);

    const auto fillStart =
      Vec2(leftUp->getPosition().x + leftUp->getContentSize().width, leftUp->getPosition().y);

    const auto fillEnd =
      Vec2(rightUp->getPosition().x - rightUp->getContentSize().width, rightUp->getPosition().y);

    auto fillAtX = fillStart.x;
    auto continueLoop = true;
    do
    {
      const auto up = Sprite::createWithSpriteFrameName("14_window_up.png");
      UTILS_BREAK_IF(up == nullptr);
      addChild(up);
      up->setPosition(fillAtX, rightUp->getPosition().y);

      const auto down = Sprite::createWithSpriteFrameName("14_window_down.png");
      UTILS_BREAK_IF(down == nullptr);
      addChild(down);
      down->setPosition(fillAtX, rightDown->getPosition().y);

      auto fillAtY = leftUp->getPosition().y - leftUp->getContentSize().height;
      for (auto i = 0; i < verticalSegments; ++i)
      {
        const auto fill = Sprite::createWithSpriteFrameName("14_window_fill.png");
        UTILS_BREAK_IF(fill == nullptr);
        addChild(fill);
        fill->setPosition(fillAtX, fillAtY);
        fillAtY -= fill->getContentSize().height;
      }

      fillAtX += up->getContentSize().width;
      if (fillAtX > fillEnd.x)
      {
        continueLoop = false;
      }
    } while (continueLoop);

    auto fillAtY = leftUp->getPosition().y - leftUp->getContentSize().height;
    for (auto i = 0; i < verticalSegments; ++i)
    {
      const auto left = Sprite::createWithSpriteFrameName("14_window_left.png");
      UTILS_BREAK_IF(left == nullptr);
      addChild(left);
      left->setPosition(leftUp->getPosition().x, fillAtY);

      const auto right = Sprite::createWithSpriteFrameName("14_window_right.png");
      UTILS_BREAK_IF(right == nullptr);
      addChild(right);
      right->setPosition(rightUp->getPosition().x, fillAtY);

      fillAtY -= left->getContentSize().height;
    }

    const auto header = Sprite::createWithSpriteFrameName("11_message_header.png");
    UTILS_BREAK_IF(header == nullptr);

    const auto headerPos = Vec2(0.f, realHeight / 2);

    header->setPosition(headerPos);
    header->setColor(Color3B(0, 127, 127));
    header->setOpacity(255);
    header->setCascadeOpacityEnabled(true);

    addChild(header, 100);

    _labelTitle = Label::createWithTTF(title, "fonts/tahoma.ttf", 150);
    UTILS_BREAK_IF(_labelTitle == nullptr);

    _labelTitle->setTextColor(Color4B(0, 255, 255, 255));
    _labelTitle->enableOutline(Color4B(0, 0, 0, 255), 5);
    _labelTitle->enableShadow(Color4B(255, 255, 255, 127), Size(5, -5));

    _labelTitle->setPosition(header->getContentSize().width / 2, (header->getContentSize().height / 2) + 100);

    header->addChild(_labelTitle, 100);

    setContentSize(Size(realWidth, realHeight));
    setCascadeOpacityEnabled(true);

    for (const auto child : getChildren())
    {
      // do not change color on color layers
      if (dynamic_cast<LayerColor*>(child) == nullptr)
      {
        child->setColor(color);
      }
    }

    ret = true;
  } while (false);
  return ret;
}
