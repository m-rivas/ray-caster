#pragma once

#include <SFML/Graphics.hpp>
#include <stdexcept>
#include "common.hpp"

class Polygon {
public:
  Polygon(sf::Color color, Coordinates coordinates, Coordinate offset);
  sf::ConvexShape GetConvexShape();
  void Move(Direction dir, float magnitude);
  void SetCoordinates(Coordinates coordinates);
  void SetColor(sf::Color color);
  void SetWidth(float width);
  void SetHeight(float height);

private:
  sf::Color color;
  Coordinates coordinates;
  Coordinate offset;
  sf::ConvexShape convex;
  float width, height;

  void InitializeConvexShape();
};