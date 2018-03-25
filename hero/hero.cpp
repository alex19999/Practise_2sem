#include<SFML/Graphics.hpp>
#include<iostream>
#include<math.h>
#include<cstdio>
#include<cstdlib>
#include<list>

#define SIZE_X 800
#define SIZE_Y 600

#define BORDER 50
#define PI 3.14159f
#define VELOCITY 150.0f

#define ALONG 1
#define AGAINST 2 
#define UP 3
#define DOWN 4

class Bullet {
    private:
        float x; // coordinates
        float y;
        sf::Vector2f velocity;
        sf::Sprite bullet;

    public:
        Bullet(); //default
        Bullet(float coord_x, float coord_y, const sf::Vector2f& vel, const sf::Texture& texture, float scale = 5.0f) : // c-tor
               x(coord_x), y(coord_y), velocity(vel), bullet(texture) 
        { 
            bullet.setOrigin(scale, scale); 
            bullet.setPosition(x, y);
        };
        
        void run(sf::Time time); // running our bullets
        float get_x () const { return x; } 
        float get_y () const { return y; }
        sf::Sprite get_bullet() const { return bullet; }
        void init(); // init position and color
        bool check(const sf::Window& window) const; // check if the bullet is on the window
};

bool Bullet::check(const sf::Window& window) const {
    if(x < 0 || y < 0 || x > window.getSize().x || y > window.getSize().y)
        return false;
    return true;
}

void Bullet::run(sf::Time time) {
    x += velocity.x * time.asSeconds();
    y += velocity.y * time.asSeconds();
    std::cout << x << " " << y << "\n";
    bullet.setPosition(x, y);
}

// necessary checks
bool may_move(const sf::Sprite& circle, int direction) {
	sf::Vector2f center = circle.getPosition();
	if(center.x < BORDER && direction == AGAINST) return false;
	if(center.x > SIZE_X - BORDER && direction == ALONG) return false; 
	if(center.y < BORDER && direction == DOWN) return false;
	if(center.y > SIZE_Y - BORDER && direction == UP) return false;
	return true;
}

void draw_bullet(const Bullet& bullet, sf::RenderWindow& window) {
    window.draw(bullet.get_bullet());
}

void run_bullets(const std::list<Bullet>& bullets, sf::Time time) {
    if(bullets.empty())
        return;
    for(auto it : bullets)
        it.run(time);
}

void shooting(std::list<Bullet>& bullets, sf::RenderWindow& window) {
    if(bullets.empty())
        return;
    for(auto it : bullets)
        draw_bullet(it, window);
}

int main() {
	sf::RenderWindow window(sf::VideoMode(SIZE_X, SIZE_Y), "My window");
	sf::Texture texture;
    sf::Texture texture_;
	sf::Event event;
    sf::Clock clock;
    sf::Time time;
    std::list<Bullet> bullets;
	texture.loadFromFile("Belarus.png");
    texture_.loadFromFile("bullet.png");
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

        time = clock.getElapsedTime();
        clock.restart();
		
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
                    {
                        std::cout << "d.x = " << d.x;
                        bullets.push_front(Bullet(circle.getPosition().x, circle.getPosition().y, 
                                           sf::Vector2f(d.x * VELOCITY, d.y * VELOCITY), texture_));
						std::cout << "hui\n";
                    }
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    {
			            sf::RectangleShape line1(sf::Vector2f(SIZE_X * SIZE_Y, 3));
			            line1.setFillColor(sf::Color::Red);
			            line1.setPosition(circle.getPosition().x, circle.getPosition().y);
			            line1.setRotation(atan2f(d.y, d.x) * 180 / PI);
			            window.draw(line1);
                    }
                    break;
                default:
                    break;
            }
		}
        run_bullets(bullets, time);
        shooting(bullets, window);

		window.display();
	}
	return 0;
}
