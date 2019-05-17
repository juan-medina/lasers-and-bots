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

#include "pause_window.h"
#include "game_ui.h"
#include "../scenes/game_scene.h"
#include "../laser_and_bots_app.h"
#include "../utils/audio/audio_helper.h"
#include "resizable_window.h"

pause_window::pause_window():
  audio_helper_(nullptr),
  current_text_button_y_(0),
  current_image_button_x_(0),
  toggle_music_item_(nullptr),
  toggle_sfx_item_(nullptr)
{
}

pause_window* pause_window::create(audio_helper* audio_helper)
{
  pause_window* ret = nullptr;

  do
  {
    auto object = new pause_window();
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

bool pause_window::init(audio_helper* audio_helper)
{
  auto ret = false;

  do
  {
    audio_helper_ = audio_helper;
    UTILS_BREAK_IF(!base_class::init());

    audio_helper_->pre_load_effect("sounds/select.mp3");

    const auto& size = Director::getInstance()->getVisibleSize();

    const auto dark_all = LayerColor::create(Color4B(0, 0, 0, 127));
    UTILS_BREAK_IF(dark_all == nullptr);

    addChild(dark_all, 0);

    const auto background = resizable_window::create("Pause", 1300.f, 1500.0f);
    UTILS_BREAK_IF(background == nullptr);

    setCascadeOpacityEnabled(true);

    background->setPosition(size.width / 2, size.height / 2);
    background->setColor(Color3B(0, 255, 255));

    addChild(background, 100);

    UTILS_BREAK_IF(!add_text_button("Exit", CC_CALLBACK_0(pause_window::on_exit, this)));
    UTILS_BREAK_IF(!add_text_button("Resume", CC_CALLBACK_0(pause_window::on_resume, this)));
    UTILS_BREAK_IF(!add_text_button("Restart", CC_CALLBACK_0(pause_window::on_reload, this)));

    toggle_sfx_item_ = add_image_toggle_button("13_sound", CC_CALLBACK_0(pause_window::on_sfx_toggle, this));
    UTILS_BREAK_IF(toggle_sfx_item_ == nullptr);

    toggle_music_item_ = add_image_toggle_button("12_music", CC_CALLBACK_0(pause_window::on_music_toggle, this));
    UTILS_BREAK_IF(toggle_music_item_ == nullptr);

    const auto menu = Menu::createWithArray(buttons_);
    UTILS_BREAK_IF(menu == nullptr);

    const auto gap_menu_y = -background->getContentSize().height / 2;

    menu->setPosition(size.width / 2, (size.height / 2) + gap_menu_y);

    addChild(menu, 100);

    setOpacity(0);
    setVisible(false);

    ret = true;
  }
  while (false);

  return ret;
}

void pause_window::display()
{
  const auto music_muted = audio_helper_->get_music_muted();
  const auto effects_muted = audio_helper_->get_effects_muted();

  toggle_music_item_->setSelectedIndex(music_muted ? 0 : 1);
  toggle_sfx_item_->setSelectedIndex(effects_muted ? 0 : 1);

  const auto fade_in_window = FadeTo::create(0.5f, 190);
  const auto show = Show::create();
  const auto fade_show = Sequence::create(show, fade_in_window, nullptr);

  runAction(fade_show);
}

void pause_window::hide()
{
  const auto fade_out_window = FadeTo::create(0.5f, 0);
  const auto hide = Hide::create();
  const auto fade_hide = Sequence::create(fade_out_window, hide, nullptr);

  runAction(fade_hide);
}

void pause_window::on_music_toggle()
{
  const auto music_muted = toggle_music_item_->getSelectedIndex() == 0;
  const auto ui = dynamic_cast<game_ui*>(getParent());
  const auto game = dynamic_cast<game_scene*>(ui->getParent());
  auto app = dynamic_cast<laser_and_bots_app*>(game->get_application());

  app->set_music_muted(music_muted);
  audio_helper_->play_effect("sounds/select.mp3");
}

void pause_window::on_sfx_toggle()
{
  const auto sfx_muted = toggle_sfx_item_->getSelectedIndex() == 0;
  const auto ui = dynamic_cast<game_ui*>(getParent());
  const auto game = dynamic_cast<game_scene*>(ui->getParent());
  auto app = dynamic_cast<laser_and_bots_app*>(game->get_application());

  app->set_effects_muted(sfx_muted);
  audio_helper_->play_effect("sounds/select.mp3");
}

void pause_window::on_resume()
{
  auto ui = dynamic_cast<game_ui*>(getParent());
  ui->on_pause(this);
}

void pause_window::on_reload()
{
  auto ui = dynamic_cast<game_ui*>(getParent());
  ui->on_reload(this);
}

void pause_window::on_exit()
{
  auto ui = dynamic_cast<game_ui*>(getParent());
  ui->on_close(this);
}

void pause_window::add_button(MenuItem* item, const ccMenuCallback& callback)
{
  item->setCallback(callback);
  buttons_.pushBack(item);
}

bool pause_window::add_text_button(const std::string& text, const ccMenuCallback& callback)
{
  auto result = false;

  do
  {
    const auto sprite = Sprite::createWithSpriteFrameName("08_Text_1.png");
    UTILS_BREAK_IF(sprite == nullptr);
    sprite->setOpacity(190);

    const auto sprite_click = Sprite::createWithSpriteFrameName("08_Text_2.png");
    UTILS_BREAK_IF(sprite_click == nullptr);
    sprite_click->setOpacity(190);

    auto item = MenuItemSprite::create(sprite, sprite_click);
    UTILS_BREAK_IF(item == nullptr);

    item->setPosition(0, current_text_button_y_);

    const auto label_button = Label::createWithTTF(text, "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(label_button == nullptr);

    label_button->setPosition(sprite->getContentSize().width / 2,
                              sprite->getContentSize().height / 2 + 30);
    label_button->setTextColor(Color4B(255, 255, 255, 255));
    label_button->enableOutline(Color4B(0, 0, 0, 255), 5);

    item->addChild(label_button, 100);

    static const auto button_gap_y = 100.f;
    current_text_button_y_ += (sprite->getContentSize().height + button_gap_y);

    add_button(item, callback);

    result = true;
  }
  while (false);

  return result;
}

MenuItemToggle* pause_window::add_image_toggle_button(const std::string& base_image, const ccMenuCallback& callback)
{
  MenuItemToggle* result = nullptr;

  do
  {
    const auto sprite_normal = Sprite::createWithSpriteFrameName(string_format("%s%s", base_image.c_str(), "_01.png"));
    UTILS_BREAK_IF(sprite_normal == nullptr);
    sprite_normal->setOpacity(190);

    const auto sprite_normal_click = Sprite::createWithSpriteFrameName(
      string_format("%s%s", base_image.c_str(), "_02.png"));
    UTILS_BREAK_IF(sprite_normal_click == nullptr);
    sprite_normal_click->setOpacity(190);

    const auto item = MenuItemSprite::create(sprite_normal, sprite_normal_click);
    UTILS_BREAK_IF(item == nullptr);

    const auto sprite_selected = Sprite::
      createWithSpriteFrameName(string_format("%s%s", base_image.c_str(), "_03.png"));
    UTILS_BREAK_IF(sprite_selected == nullptr);
    sprite_selected->setOpacity(190);

    const auto sprite_selected_click = Sprite::createWithSpriteFrameName(
      string_format("%s%s", base_image.c_str(), "_02.png"));
    UTILS_BREAK_IF(sprite_selected_click == nullptr);
    sprite_selected_click->setOpacity(190);

    const auto item_selected = MenuItemSprite::create(sprite_selected, sprite_selected_click);
    UTILS_BREAK_IF(item_selected == nullptr);

    if (current_image_button_x_ == 0)
    {
      current_image_button_x_ = (buttons_.front()->getContentSize().width / 2) - (sprite_normal->getContentSize().width
        / 2);
    }
    else
    {
      current_image_button_x_ = -current_image_button_x_;
    }

    const auto item_toggle = MenuItemToggle::create(item);
    UTILS_BREAK_IF(item_toggle == nullptr);
    item_toggle->addSubItem(item_selected);

    item_toggle->setPosition(current_image_button_x_, current_text_button_y_);

    add_button(item_toggle, callback);

    result = item_toggle;
  }
  while (false);

  return result;
}
