#include <SFML\Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <algorithm>

const int K = 20;
const int IterationCount = 50;

using Centroids = std::vector<sf::Color>;
using ClosestCentroidIndices = std::vector<int>;


struct ColorBuf {
	int r, g, b;
	int count;
	ColorBuf& operator += (const sf::Color c)
	{
		count++;
		r += c.r;
		b += c.b;
		g += c.g;
		return *this;
	}
	ColorBuf& normalize()
	{
		r /= count;
		b /= count;
		g /= count;
		return *this;
	}
};

int getDistance(int x1, int x2, int y1, int y2, int z1, int z2)
{
	return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2);
}

Centroids initializeCentroids(const sf::Image& im, int k)
{
	Centroids c;
	c.resize(k);
	for (int i = 0; i < k; i++)
	{
		sf::Vector2u pix;
		pix.x = rand() % im.getSize().x;
		pix.y = rand() % im.getSize().y;
		c[i] = im.getPixel(pix.x, pix.y);
	}
	return c;
}

ClosestCentroidIndices findClosestCentroids(const sf::Image& im, Centroids centroids)
{
	int width = im.getSize().x;
	int height = im.getSize().y;

	ClosestCentroidIndices indices;
	indices.resize(width * height);

	std::vector<int> distance;
	distance.resize(K);

	for (int i = 0; i < height; i++) 
	{
		for (int j = 0; j < width; j++) 
		{
			for (int k = 0; k < K; k++)
			{
				distance[k] = getDistance(im.getPixel(j, i).r, centroids[k].r,
					im.getPixel(j, i).g, centroids[k].g, 
					im.getPixel(j, i).b, centroids[k].b);
			}

			int min = distance[0];
			int index = 0;

			for (int k = 0; k < K; k++) 
			{
				if (min > distance[k]) 
				{
					min = distance[k];
					index = k;
				}
			}

			indices[i * width + j] = index;
		}
	}
	return indices;
}

Centroids computeMeans(const sf::Image& im, ClosestCentroidIndices ind, int k)
{
	Centroids c;
	c.resize(k);

	std::vector<ColorBuf> buf;
	buf.resize(k);

	int width = im.getSize().x;

	for (int i = 0; i < ind.size(); i++)
	{
		buf[ind[i]] += im.getPixel(i % width, i / width);
	}

	for (int i = 0; i < k; i++)
	{
		buf[i].normalize();
	}

	for (int i = 0; i < k; i++)
	{
		c[i] = sf::Color(buf[i].r, buf[i].g, buf[i].b);
	}

	return c;
}

void changeColors(sf::Image& im, ClosestCentroidIndices ids, Centroids c)
{
	int width = im.getSize().x;

	for (int i = 0; i < ids.size(); i++)
	{
		im.setPixel(i % width, i / width, c[ ids[i] ] );
	}
}


int main()
{
	sf::Texture texture;
	texture.loadFromFile("example.png");

	auto image = texture.copyToImage();

	ClosestCentroidIndices ids;
	Centroids centroids = initializeCentroids(image, K);

	for (int it = 0; it < IterationCount; it++)
	{
		ids = findClosestCentroids(image, centroids);
		centroids = computeMeans(image, ids, K);
	}

	changeColors(image, ids, centroids);

	image.saveToFile("result.png");

	sf::Sprite photo;
	photo.setTexture(texture);
	sf::Vector2f size = (sf::Vector2f)photo.getTexture()->getSize();
	sf::RenderWindow window(sf::VideoMode(2 * size.x, size.y), "My window");
	
	sf::Texture result;
	result.loadFromFile("result.png");
	sf::Sprite resultPhoto;
	resultPhoto.setTexture(result);
	resultPhoto.setPosition(size.x, 0);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.draw(resultPhoto);
		window.draw(photo);
		window.display();
	}
	return 0;
}
