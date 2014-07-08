#pragma once
#include <sstream>


namespace sf
{
	class Sprite;
	class Text;
	class RectangleShape;
}

// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.
template <typename T>
std::string toString(const T& value);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
void centerOrigin(sf::RectangleShape& rect);

#include "Utility.inl"
