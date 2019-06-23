#include "message_window.h"
#include "../ui/TextButton.h"
#include "../utils/audio/AudioHelper.h"

message_window::message_window() : audio_helper_(nullptr), continue_item_(nullptr), sub_label_(nullptr) {}

message_window* message_window::create(AudioHelper* audio_helper)
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
  } while (false);

  return ret;
}

bool message_window::init(AudioHelper* audio_helper)
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

    UTILS_BREAK_IF(!base_class::init("message", audio_helper, 1800.f, 1300.f, animation_type::fade));

    sub_label_ = Label::createWithTTF("", "fonts/tahoma.ttf", 100);
    UTILS_BREAK_IF(sub_label_ == nullptr);

    sub_label_->setTextColor(Color4B(255, 255, 255, 255));
    sub_label_->enableOutline(Color4B(0, 0, 0, 255), 5);

    sub_label_->setPosition(0.f, (getContentSize().height / 2) - 250);

    addChild(sub_label_, 100);

    ret = true;
  } while (false);
  return ret;
}

void message_window::display(const std::string& message, const std::string& sub_message,
                             const ccMenuCallback& callback)
{
  base_class::display();

  set_title(message);
  sub_label_->setString(sub_message);
  continue_item_->setCallback(callback);
}

void message_window::hide()
{
  base_class::hide();
}

bool message_window::create_menu_items()
{
  auto result = false;
  do
  {
    continue_item_ = add_text_button("Continue", nullptr);
    UTILS_BREAK_IF(continue_item_ == nullptr);

    set_default_menu_item(continue_item_);

    result = true;
  } while (false);

  return result;
}
