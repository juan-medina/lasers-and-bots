#include "resizable_window.h"

resizable_window::resizable_window() : label_title_(nullptr) {}

resizable_window* resizable_window::create(const std::string& title, const float width, const float height,
                                           const Color3B& color /*= Color3B(0, 255, 255)*/)
{
  resizable_window* ret = nullptr;

  do
  {
    auto object = new resizable_window();
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

bool resizable_window::init(const std::string& title, const float width, const float height,
                            const Color3B& color /*= Color3B(0, 255, 255)*/)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init());

    const auto left_up = Sprite::createWithSpriteFrameName("14_window_left_up.png");
    UTILS_BREAK_IF(left_up == nullptr);
    addChild(left_up);

    const auto right_up = Sprite::createWithSpriteFrameName("14_window_right_up.png");
    UTILS_BREAK_IF(right_up == nullptr);
    addChild(right_up);

    const auto left_down = Sprite::createWithSpriteFrameName("14_window_left_down.png");
    UTILS_BREAK_IF(left_down == nullptr);
    addChild(left_down);

    const auto right_down = Sprite::createWithSpriteFrameName("14_window_right_down.png");
    UTILS_BREAK_IF(right_down == nullptr);
    addChild(right_down);

    const auto up_info = Sprite::createWithSpriteFrameName("14_window_up.png");
    UTILS_BREAK_IF(up_info == nullptr);
    addChild(up_info);

    const auto left_info = Sprite::createWithSpriteFrameName("14_window_left.png");
    UTILS_BREAK_IF(left_info == nullptr);
    addChild(left_info);

    const auto interior_width = width - left_up->getContentSize().width - right_up->getContentSize().width;
    const auto horizontal_segments = static_cast<int>(interior_width / up_info->getContentSize().width);
    const auto real_width = (horizontal_segments * up_info->getContentSize().width) +
                            left_up->getContentSize().width + right_up->getContentSize().width;

    const auto interior_height =
      height - left_up->getContentSize().height - left_down->getContentSize().height;
    const auto vertical_segments = static_cast<int>(interior_height / left_info->getContentSize().height);
    const auto real_height = (vertical_segments * left_info->getContentSize().height) +
                             left_up->getContentSize().height + left_down->getContentSize().height;

    removeChild(up_info);
    removeChild(left_info);

    const auto origin = Vec2(-real_width / 2, real_height / 2);
    const auto dest = origin + Vec2(real_width, -real_height);

    left_up->setPosition(origin.x + left_up->getContentSize().width / 2,
                         origin.y - left_up->getContentSize().height / 2);

    right_up->setPosition(dest.x - right_up->getContentSize().width / 2,
                          origin.y - right_up->getContentSize().height / 2);

    left_down->setPosition(origin.x + left_down->getContentSize().width / 2,
                           dest.y + left_down->getContentSize().height / 2);

    right_down->setPosition(dest.x - right_down->getContentSize().width / 2,
                            dest.y + right_down->getContentSize().height / 2);

    const auto fill_start =
      Vec2(left_up->getPosition().x + left_up->getContentSize().width, left_up->getPosition().y);

    const auto fill_end =
      Vec2(right_up->getPosition().x - right_up->getContentSize().width, right_up->getPosition().y);

    auto fill_at_x = fill_start.x;
    auto continue_loop = true;
    do
    {
      const auto up = Sprite::createWithSpriteFrameName("14_window_up.png");
      UTILS_BREAK_IF(up == nullptr);
      addChild(up);
      up->setPosition(fill_at_x, right_up->getPosition().y);

      const auto down = Sprite::createWithSpriteFrameName("14_window_down.png");
      UTILS_BREAK_IF(down == nullptr);
      addChild(down);
      down->setPosition(fill_at_x, right_down->getPosition().y);

      auto fill_at_y = left_up->getPosition().y - left_up->getContentSize().height;
      for (auto i = 0; i < vertical_segments; ++i)
      {
        const auto fill = Sprite::createWithSpriteFrameName("14_window_fill.png");
        UTILS_BREAK_IF(fill == nullptr);
        addChild(fill);
        fill->setPosition(fill_at_x, fill_at_y);
        fill_at_y -= fill->getContentSize().height;
      }

      fill_at_x += up->getContentSize().width;
      if (fill_at_x > fill_end.x)
      {
        continue_loop = false;
      }
    } while (continue_loop);

    auto fill_at_y = left_up->getPosition().y - left_up->getContentSize().height;
    for (auto i = 0; i < vertical_segments; ++i)
    {
      const auto left = Sprite::createWithSpriteFrameName("14_window_left.png");
      UTILS_BREAK_IF(left == nullptr);
      addChild(left);
      left->setPosition(left_up->getPosition().x, fill_at_y);

      const auto right = Sprite::createWithSpriteFrameName("14_window_right.png");
      UTILS_BREAK_IF(right == nullptr);
      addChild(right);
      right->setPosition(right_up->getPosition().x, fill_at_y);

      fill_at_y -= left->getContentSize().height;
    }

    const auto header = Sprite::createWithSpriteFrameName("11_message_header.png");
    UTILS_BREAK_IF(header == nullptr);

    const auto header_pos = Vec2(0.f, real_height / 2);

    header->setPosition(header_pos);
    header->setColor(Color3B(0, 127, 127));
    header->setOpacity(255);
    header->setCascadeOpacityEnabled(true);

    addChild(header, 100);

    label_title_ = Label::createWithTTF(title, "fonts/tahoma.ttf", 150);
    UTILS_BREAK_IF(label_title_ == nullptr);

    label_title_->setTextColor(Color4B(0, 255, 255, 255));
    label_title_->enableOutline(Color4B(0, 0, 0, 255), 5);
    label_title_->enableShadow(Color4B(255, 255, 255, 127), Size(5, -5));

    label_title_->setPosition(header->getContentSize().width / 2,
                              (header->getContentSize().height / 2) + 100);

    header->addChild(label_title_, 100);

    setContentSize(Size(real_width, real_height));
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
