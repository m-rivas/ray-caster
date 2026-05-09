#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "common.hpp"
#include "polygon.hpp"
#include "window.hpp"

#define DR 0.0174533 // one degree in radians

// clang-format off
std::vector<std::vector<int>> map = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                     {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                     {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                     {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                                     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                     {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
// clang-format on

struct Movement {
  bool moveForward = false;
  bool moveBackward = false;
  bool turnLeft = false;
  bool turnRight = false;
};

void getMovement(Movement &userMoving) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    userMoving.moveForward = true;
  else
    userMoving.moveForward = false;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    userMoving.moveBackward = true;
  else
    userMoving.moveBackward = false;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    userMoving.turnLeft = true;
  else
    userMoving.turnLeft = false;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    userMoving.turnRight = true;
  else
    userMoving.turnRight = false;
}

float getDistance(sf::Vector2f first, sf::Vector2f second) {
  auto dx = first.x - second.x;
  auto dy = first.y - second.y;
  return std::sqrt(dx * dx + dy * dy);
}

void drawRays(sf::CircleShape user, sf::RectangleShape directionLine,
              sf::RenderWindow &window) {
  auto userPosition = user.getPosition();
  auto rayAngle = (2 * M_PI) - directionLine.getRotation().asRadians();

  float rayY = 0, rayX = 0, yOffset = 0, xOffset = 0;
  int mapX = 0, mapY = 0, mapPos = 0;

  sf::Vector2f hVec, vVec;

  rayAngle += DR * 30;
  if (rayAngle < 0)
    rayAngle += 2 * M_PI;
  if (rayAngle > 2 * M_PI)
    rayAngle -= 2 * M_PI;

  for (int ray = 0; ray < 60; ray++) {
    float aTan = std::atan(rayAngle);
    float invTan = 1 / std::tan(rayAngle);
    float nTan = -std::tan(rayAngle);
    float tan = std::tan(rayAngle);
    float cos = std::cos(rayAngle);

    // Horizontal Lines
    int dof = 0;
    auto lookingUp = false;
    // Looking up
    if (rayAngle < M_PI) {
      lookingUp = true;
      rayY = ((static_cast<int>(userPosition.y) / 100) * 100) - 0.0001;
      rayX = ((userPosition.y - rayY) * invTan) + userPosition.x;
      yOffset = -100;
      xOffset = (-yOffset) * invTan;
    }
    // Looking down
    else if (rayAngle > M_PI) {
      rayY = ((static_cast<int>(userPosition.y) / 100) * 100) + 100;
      rayX = ((userPosition.y - rayY) * invTan) + userPosition.x;
      yOffset = 100;
      xOffset = (-yOffset) * invTan;
    }
    // Looking exactly left or right
    else if (rayAngle == M_PI || rayAngle == (2 * M_PI)) {
      rayY = userPosition.y;
      rayX = userPosition.x;
      dof = 8;
    }
    auto appliedOffset = 0;
    while (dof < 8) {
      mapX = static_cast<int>(rayX) / 100;
      mapY = static_cast<int>(rayY) / 100;
      if (mapX < map[0].size() && mapY < map.size() && map[mapY][mapX]) {
        dof = 8;
      } else {
        appliedOffset++;
        rayX += xOffset;
        rayY += yOffset;
        dof++;
      }
    }

    hVec = sf::Vector2f({rayX, rayY});

    aTan = std::atan(rayAngle);
    invTan = 1 / std::tan(rayAngle);
    nTan = -std::tan(rayAngle);
    tan = std::tan(rayAngle);
    cos = std::cos(rayAngle);
    dof = 0;
    // Looking left
    if (rayAngle > M_PI / 2 && rayAngle < (3 * M_PI) / 2) {
      rayX = ((static_cast<int>(userPosition.x) / 100) * 100) - 0.001;
      rayY = ((userPosition.x - rayX) * tan) + userPosition.y;
      xOffset = -100;
      yOffset = (-xOffset) * tan;
    }
    // Looking right
    else if (rayAngle < M_PI / 2 || rayAngle > (3 * M_PI) / 2) {
      rayX = ((static_cast<int>(userPosition.x) / 100) * 100) + 100;
      rayY = ((userPosition.x - rayX) * tan) + userPosition.y;
      xOffset = 100;
      yOffset = (-xOffset) * tan;
    }
    // Looking exactly up or down
    else if (rayAngle == M_PI / 2 || rayAngle == (3 * M_PI) / 2) {
      rayY = userPosition.y;
      rayX = userPosition.x;
      dof = 8;
    }
    appliedOffset = 0;
    while (dof < 8) {
      mapX = static_cast<int>(rayX) / 100;
      mapY = static_cast<int>(rayY) / 100;
      if (mapX < map[0].size() && mapY < map.size() && map[mapY][mapX]) {
        dof = 8;
      } else {
        appliedOffset++;
        rayX += xOffset;
        rayY += yOffset;
        dof++;
      }
    }

    vVec = sf::Vector2f({rayX, rayY});
    rayAngle -= DR;
    sf::VertexArray drawRay(sf::PrimitiveType::Lines, 2);
    drawRay[0].position = user.getPosition();
    if (getDistance(userPosition, hVec) > getDistance(userPosition, vVec))
      drawRay[1].position = vVec; // getRayEnd(user, userDirectionLine);
    else
      drawRay[1].position = hVec; // getRayEnd(user, userDirectionLine);
    drawRay[0].color = sf::Color::Black;
    drawRay[1].color = sf::Color::Black;
    window.draw(drawRay);
  }
}

int main() {
  // Standard Application Setup
  auto window = InitializeWindow("Top Down View");

  int tilesize = 100;
  int userRadius = 10;
  sf::CircleShape user(userRadius);
  auto userColor = sf::Color(210, 52, 235);
  auto userPosition =
      sf::Vector2f((APP_RESOLUTION.width / 2), (APP_RESOLUTION.height / 2));
  user.setOrigin(sf::Vector2f(userRadius, userRadius));
  user.setFillColor(userColor);
  user.setPosition(userPosition);

  sf::Angle userAngle = sf::degrees(30);
  auto userSpeed{2.0f};
  sf::Angle turningSpeed = sf::degrees(1);

  auto lineThickness{4.0f};
  sf::RectangleShape userDirectionLine({100.0f, lineThickness});
  userDirectionLine.setRotation(userAngle);
  userDirectionLine.setOrigin({0, lineThickness / 2});
  userDirectionLine.setPosition(user.getPosition());
  userDirectionLine.setFillColor(userColor);
  Movement userMoving;
  sf::Vector2f debugVector{0, 0};
  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>() ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        window.close();
      }
    }
    window.clear(sf::Color::Black);

    // Draw Map
    int i = 0, j = 0;
    for (auto row : map) {
      for (auto square : row) {
        sf::RectangleShape squareFill(sf::Vector2f(tilesize, tilesize));
        auto position = sf::Vector2f(j * tilesize, i * tilesize);
        squareFill.setPosition(position);
        squareFill.setOutlineThickness(1.0f);
        squareFill.setOutlineColor(sf::Color::Black);

        if (square)
          squareFill.setFillColor(sf::Color::Green);
        else
          squareFill.setFillColor(sf::Color::White);
        window.draw(squareFill);
        j++;
      }
      j = 0;
      i++;
    }

    // Move user
    auto userMovement = sf::Vector2f(userSpeed, userAngle);

    getMovement(userMoving);

    if (userMoving.moveForward) {
      userDirectionLine.move(userMovement);
      user.move(userMovement);
    }
    if (userMoving.moveBackward) {
      userDirectionLine.move(-userMovement);
      user.move(-userMovement);
    }
    if (userMoving.turnLeft) {
      userAngle -= turningSpeed;
      userDirectionLine.setRotation(userAngle);
    }
    if (userMoving.turnRight) {
      userAngle += turningSpeed;
      userDirectionLine.setRotation(userAngle);
    }

    // Draw user circle
    window.draw(user);

    // Draw user direction line
    window.draw(userDirectionLine);

    // Draw ray line
    drawRays(user, userDirectionLine, window);

    window.display();
  }
  return 0;
}
