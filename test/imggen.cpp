#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>

static unsigned int width = 600, height = 600;

int main() {
  sf::Image image;
  image.create(width, height, sf::Color::Black);

  // replace with actual code
  while (true) {
    for (int x = 0; x < width; x++) {
      for (int y = 0; y < height; y++) {
        image.setPixel(x, y,
                       sf::Color(rand() % 255, rand() % 255, rand() % 255));
      }
    }
    image.saveToFile("bar.png");
  }
}