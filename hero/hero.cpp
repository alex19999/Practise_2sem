#include<SFML/Graphics.hpp>
#include<iostream>
#include<math.h>

#define SIZE_X 800
#define SIZE_Y 600

#define BORDER 50
#define PI 3.14159f

#define ALONG 1
#define AGAINST 2 
#define UP 3
#define DOWN 4

bool may_move(const  sf::Sprite& circle, int direction) {
	sf::Vector2f center = circle.getPosition();
	if(center.x < BORDER && direction == AGAINST) return false;
	if(center.x > SIZE_X - BORDER && direction == ALONG) return false; 
	if(center.y < BORDER && direction == DOWN) return false;
	if(center.y > SIZE_Y - BORDER && direction == UP) return false;
	return true;
}

void shoot_bullets(const sf::Sprite& circle) {
	sf::Texture texture;
	texture.loadFromFile("bullet.png");
	sf::Sprite bullet(texture);
	bullet.setScale(0.5f, 0.5f);
	bullet.setPosition(circle.getPosition().x, circle.getPosition().y);
	sf::Time time = clock.getElapsedTime();
	circle.setPosition(5 * time.asSeconds(), 5 * time.asSeconds())
}

int main() {
	sf::RenderWindow window(sf::VideoMode(SIZE_X, SIZE_Y), "My window");
	sf::Texture texture;
	sf::Event event;
	texture.loadFromFile("Belarus.png");
	sf::Sprite circle(texture);
	circle.setScale(0.5f, 0.5f);
	circle.setPosition(SIZE_X / 2, SIZE_Y / 2);
	while(window.isOpen()) {
		window.clear(sf::Color::Blue);
		window.draw(circle);
		sf::Vector2u circleSize = circle.getTexture()->getSize();
		circle.setOrigin(circleSize.x / 2, circleSize.y / 2);

		sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
		sf::Vector2f center = circle.getPosition();
		sf::Vector2f d = sf::Vector2f(mousePosition.x, mousePosition.y) - center;

		circle.setRotation(90 + atan2f(d.y, d.x) * 180 / PI);
		
		while(window.pollEvent(event)) {
			switch(event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					if(event.key.code == sf::Keyboard::A)
						if(may_move(circle, AGAINST))
							circle.move(-10, 0);
					if(event.key.code == sf::Keyboard::D)
						if(may_move(circle, ALONG))
							circle.move(10, 0);
					if(event.key.code == sf::Keyboard::W)
						if(may_move(circle, DOWN))
							circle.move(0, -10);
					if(event.key.code == sf::Keyboard::S)
						if(may_move(circle, UP))
							circle.move(0, 10);
					break;
				case sf::Event::MouseButtonPressed:
					if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
						std::cout << "hui\n";
					break;
				default:
					break;
			}
		}
		window.display();
	}
	return 0;
}
