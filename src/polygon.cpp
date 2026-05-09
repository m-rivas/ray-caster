#include "polygon.hpp"

Polygon::Polygon(sf::Color color, Coordinates coordinates, Coordinate offset)
    : color(color), coordinates(coordinates), offset(offset) {
  InitializeConvexShape();
}

sf::ConvexShape Polygon::GetConvexShape() {
  return convex;
}

void Polygon::Move(Direction dir, float magnitude) {
  Coordinate dest{offset.first, offset.second};
  switch (dir) {

  case UP:
    dest.second = offset.second - magnitude;
    if (dest.second < 0)
      return;
    offset.second = dest.second;
    break;

  case DOWN:
    dest.second = offset.second + magnitude;
    if (dest.second > APP_RESOLUTION.height ||
        dest.second + height > APP_RESOLUTION.height)
      return;
    offset.second = dest.second;
    break;

  case LEFT:
    dest.first = offset.first - magnitude * 2;
    if (dest.first < 0)
      return;
    offset.first = dest.first;
    break;

  case RIGHT:
    dest.first = offset.first + magnitude * 2;
    if (dest.first > APP_RESOLUTION.width ||
        dest.first + width > APP_RESOLUTION.width)
      return;
    offset.first = dest.first;
    break;
  }
  convex.setPosition({dest.first, dest.second});
}

void Polygon::SetCoordinates(Coordinates coordinates) {
  std::size_t index{0};
  for (auto coord : coordinates) {
    convex.setPoint(index, sf::Vector2f(coord.first, coord.second));
    index++;
  }
}

void Polygon::SetColor(sf::Color color) {
  this->color = color;
  convex.setFillColor(this->color);
}

void Polygon::SetWidth(float width) {
  if (width < 0)
    throw std::runtime_error("Cannot set width of polygon to zero");
  this->width = width;
}

void Polygon::SetHeight(float height) {
  if (height < 0)
    throw std::runtime_error("Cannot set height of polygon to zero");
  this->height = height;
}

void Polygon::InitializeConvexShape() {
  convex.setPointCount(coordinates.size());
  convex.setFillColor(color);
  convex.setPosition({offset.first, offset.second});
  SetCoordinates(coordinates);
}