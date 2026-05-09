#pragma once

#include <SFML/Graphics.hpp>

namespace raycaster {

struct Movement {
  bool moveForward = false;
  bool moveBackward = false;
  bool turnLeft = false;
  bool turnRight = false;
};

class User {
public:
  explicit User() {};
  User(const User &) = delete;
  User &operator=(const User &) = delete;
  User(User &&) noexcept = default;
  User &operator=(User &&) noexcept = default;
  ~User() = default;

  void example();
  void setColor(sf::Color color);
  [[nodiscard]] sf::Color getColor();

private:
  void getMovement(Movement &userMoving);
  sf::Color userColor;
};

} // namespace raycaster
