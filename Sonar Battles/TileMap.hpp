#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <array>

class TileMap : public sf::Drawable, public sf::Transformable
{
public:

	template<std::size_t SIZE>
	bool load(const sf::Texture& tileSet, unsigned int tileSize, const std::array<unsigned int, SIZE> tiles, unsigned int width, unsigned int height)
	{
		mTileSet = tileSet;
		mGridSize.x = width;
		mGridSize.y = height;
		mTileSize = tileSize;

		mVertices.setPrimitiveType(sf::Quads);
		mVertices.resize(mGridSize.x * mGridSize.y * 4); //Set vertex count to be 4 (corners) * total number of squares

		// Setup the vertex array
		for (unsigned int i = 0; i != mGridSize.x; ++i)
		{
			for (unsigned int j = 0; j != mGridSize.y; ++j)
			{
				// Get the current tile number
				int tileNumber = tiles[i + j * mGridSize.x];

				// Find it's position in the tileset texture
				int tu = tileNumber % (mTileSet.getSize().x / mTileSize);
				int tv = tileNumber / (mTileSet.getSize().x / mTileSize);

				// Get pointer to current tile's quad
				sf::Vertex* quad = &mVertices[(i + j * mGridSize.x) * 4];

				// Define its 4 corners
				quad[0].position = sf::Vector2f(i * mTileSize, j * mTileSize);			//	Top Left
				quad[1].position = sf::Vector2f((i + 1) * mTileSize, j * mTileSize);			//	Top Right
				quad[2].position = sf::Vector2f((i + 1) * mTileSize, (j + 1) * mTileSize);		//	Bottom Right
				quad[3].position = sf::Vector2f(i * mTileSize, (j + 1) * mTileSize);		//	Bottom Left

				// Define its 4 texture coordinates
				quad[0].texCoords = sf::Vector2f(tu * mTileSize, tv * mTileSize);
				quad[1].texCoords = sf::Vector2f((tu + 1) * mTileSize, tv * mTileSize);
				quad[2].texCoords = sf::Vector2f((tu + 1) * mTileSize, (tv + 1) * mTileSize);
				quad[3].texCoords = sf::Vector2f(tu * mTileSize, (tv + 1) * mTileSize);
			}
		}

		return true;
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		// apply the tileset texture
		states.texture = &mTileSet;

		// draw the vertex array
		target.draw(mVertices, states);
	}

	sf::Texture							mTileSet;
	sf::VertexArray						mVertices;
	sf::Vector2u						mGridSize;
	int								mTileSize;

};