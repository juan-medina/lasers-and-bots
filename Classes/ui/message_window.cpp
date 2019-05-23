#include "message_window.h"
#include "../utils/audio/audio_helper.h"
#include "resizable_window.h"

message_window::message_window():
  audio_helper_(nullptr),
  continue_item_(nullptr),
  sub_label_(nullptr)
{
}

message_window* message_window::create(audio_helper* audio_helper)
{
  message_window* ret = nullptr;

  do
  {
    auto object = new message_window();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(audio_helper))
    {
      object->autorelease();
    }
    else
    {
      delete object;
      object = nullptr;
    }

    ret = object;
  }
  while (false);

  return ret;
}

bool message_window::init(audio_helper* audio_helper)
{
  auto ret = false;

  do
  {
    audio_helper_ = audio_helper;

    const auto& size = Director::getInstance()->getVisibleSize();

    const auto dark_all = LayerColor::create(Color4B(0, 0, 0, 127));
    UTILS_BREAK_IF(dark_all == nullptr);

    addChild(dark_all, 0);
    dark_all->setPosition(-size.width / 2, -size.height / 2);

    UTILS_BREAK_IF(!base_class::init("message", 1800.f, 1300.f));

    setPosition(size.width / 2, size.height / 2);

    sub_label_ = Label::createWithTTF("", "fonts/tahoma.ttf", 100);
    UTILS_BREAK_IF(sub_label_ == nullptr);

    sub_label_->setTextColor(Color4B(255, 255, 255, 255));
    sub_label_->enableOutline(Color4B(0, 0, 0, 255), 5);

    sub_label_->setPosition(0.f, (getContentSize().height / 2) - 250);

    addChild(sub_label_, 100);

    const auto continue_sprite = Sprite::createWithSpriteFrameName("08_Text_1.png");
    UTILS_BREAK_IF(continue_sprite == nullptr);

    const auto continue_sprite_click = Sprite::createWithSpriteFrameName("08_Text_2.png");
    UTILS_BREAK_IF(continue_sprite_click == nullptr);

    continue_item_ = MenuItemSprite::create(continue_sprite, continue_sprite_click);
    UTILS_BREAK_IF(continue_item_ == nullptr);

    continue_item_->setPosition(0, 0);

    const auto label_button = Label::createWithTTF("Continue", "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(label_button == nullptr);

    label_button->setPosition(continue_sprite->getContentSize().width / 2,
                              continue_sprite->getContentSize().height / 2 + 30);
    label_button->setTextColor(Color4B(255, 255, 255, 255));
    label_button->enableOutline(Color4B(0, 0, 0, 255), 5);

    continue_item_->addChild(label_button, 100);

    const auto menu = Menu::create(continue_item_, nullptr);
    UTILS_BREAK_IF(menu == nullptr);

    menu->setPosition(0.f, -getContentSize().height / 2);

    addChild(menu, 100);

    setOpacity(0);
    setVisible(false);

    ret = true;
  }
  while (false);
  return ret;
}

void message_window::display(const std::string& message, const std::string& sub_message,
                             const std::string& time_message, const ccMenuCallback& callback)
{
  set_title(message);
  sub_label_->setString(sub_message);
  continue_item_->setCallback(callback);
  setVisible(true);

  const auto fade_in_message = FadeTo::create(0.5f, 190);
  runAction(fade_in_message);
}
