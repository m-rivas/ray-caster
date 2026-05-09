#include "window.hpp"
#include "common.hpp"

sf::RenderWindow InitializeWindow(std::string title) {
  auto window = sf::RenderWindow(
      sf::VideoMode({APP_RESOLUTION.width, APP_RESOLUTION.height}), title);
  window.setFramerateLimit(144);
  window.setKeyRepeatEnabled(true);
  return window;
}