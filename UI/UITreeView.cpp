#include "UITreeView.h"

UITreeView::UITreeView(sf::Vector2f p_pos, sf::Vector2f p_size)
    : list_world({p_pos, p_size}), list_view(list_world) {

  list_bg.setPosition(list_world.getPosition());
  list_bg.setSize(list_world.getSize());
  list_bg.setFillColor(sf::Color(195, 210, 226));

  list_vbox.padding.y = 10.0F;
  list_parent.add_child(list_vbox);
  list_parent.setPosition(p_pos - sf::Vector2f(5.0F, 0.0F));
}

void UITreeView::init_ui() {
  const auto win_size = static_cast<sf::Vector2f>(window.getSize());
  list_view.setViewport(
      {list_world.left / win_size.x, list_world.top / win_size.y,
       list_world.width / win_size.x, list_world.height / win_size.y});

  list_view.move({-20, -15});
}

void UITreeView::update_item_name(int index, std::string p_new_name) {
  buttons[index].item.get()->set_text(p_new_name);
}

void UITreeView::select_item_by_index(int p_idx) {
  if (p_idx < 0 || p_idx > buttons.size()) {
    return;
  }

  // Deselect all the buttons first.
  for (auto &btn : buttons) {
    btn.item.get()->set_pressed(false);
  }

  // Select only the button with the given index.
  buttons[p_idx].item.get()->set_pressed(true);
}

std::optional<int> UITreeView::index_of_item_with_id(int p_id) {
  int c = 0;
  for (const auto &[id, item] : buttons) {
    if (id == p_id) {
      return c;
    }
    c++;
  }

  return {};
}

int UITreeView::add_item(const std::string &p_text,
                         std::function<void()> clicked_callback) {

  std::shared_ptr<Button> btn(new Button(p_text));
  btn.get()->default_fill_color = sf::Color::Transparent;
  btn.get()->set_pressed(false);
  btn.get()->clicked_callback = clicked_callback;

  // Assign an unique id to every tree_item.
  tree_item button;
  button.id = global_id;
  button.item = std::move(btn);

  buttons.push_back(button);

  list_vbox.add_child(*button.item);

  global_id++;

  return button.id;
}

void UITreeView::handle_inputs(sf::Event event) {
  if (isMouseOverRect(list_world)) {
    if (event.type == sf::Event::MouseWheelMoved) {
      auto pos = list_parent.getPosition();
      auto size = list_parent.rect_size();
      bool content_overflowing =
          pos.y + size.y > list_world.top + list_world.height;

      if (content_overflowing) {
        list_view.move(0, -event.mouseWheel.delta * 5);

        constexpr int OFFSET = 15;
        sf::FloatRect view_rect = {list_view.getCenter(), list_view.getSize()};

        bool first_item_went_above_top_pos =
            (view_rect.top - view_rect.height / 2.0) < pos.y - OFFSET;

        bool last_item_went_below_bottom_pos =
            (view_rect.top + view_rect.height / 2.0) > pos.y + size.y - OFFSET;

        if (first_item_went_above_top_pos || last_item_went_below_bottom_pos) {
          // Revert back the scroll.
          list_view.move(0, +event.mouseWheel.delta * 5);
        }
      }
    }

    window.setView(list_view);
    list_parent.handle_inputs(event);
    window.setView(window.getDefaultView());
  }
}

void UITreeView::Render() {
  window.draw(list_bg);

  window.setView(list_view);
  list_parent.Render();
  window.setView(window.getDefaultView());
}