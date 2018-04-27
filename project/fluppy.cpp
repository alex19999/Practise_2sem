#include<SFML/Graphics.hpp>
#include<iostream>
#include<math.h>
#include<cstdio>
#include<cstdlib>
#include<list>

#define SIZE_X 1000
#define SIZE_Y 1000
#define G      0.08
#define speed  0.1
#define border 200

class Bird 
{
    private:
        float x;
        float y;
        sf::Vector2f velocity;
        sf::Sprite bird;
    
    public:
        Bird();
        Bird(float coord_x, float coord_y, const sf::Vector2f& vel, const sf::Texture& texture, float scale = 0.01f) : // c-tor
            x(coord_x), y(coord_y), velocity(vel), bird(texture) 
        { 
            bird.setOrigin(scale, scale); 
            bird.setPosition(x, y);
        };
        ~Bird(){};
        
        float get_x() { return x; }
        float get_y() { return y; }
        sf::Vector2f get_velocity() { return velocity; }
        sf::Sprite get_bird() const { return bird; }
        void fly();
        void apply_force(sf::Time& time);
        float is_collision();
};

class Wall
{
    private:
        float x;
        float y;
        sf::Vector2f velocity;
        sf::Sprite wall;

    public:
        Wall();
        Wall(float coord_x, float coord_y, const sf::Vector2f& vel, const sf::Texture& texture, float scale = 0.01f) : // c-tor
            x(coord_x), y(coord_y), velocity(vel), wall(texture)
        {
            wall.setOrigin(scale, scale);
            wall.scale(0.9, 0.6);
            wall.setPosition(x, coord_y);
        };
        ~Wall(){};
        float get_x() { return x; }
        float get_y() { return y; }
        void move(sf::Time& time);
        bool check();
        sf::Vector2f get_velocity() { return velocity; }
        sf::Sprite get_wall() const { return wall; }
};

void draw_wall(const Wall& wall, sf::RenderWindow& window)
{
    window.draw(wall.get_wall());
}

void draw_bird(const Bird& bird, sf::RenderWindow& window) 
{
        window.draw(bird.get_bird());
}

void Wall::move(sf::Time& time) 
{
    wall.move(sf::Vector2f(-speed, 0));
    std::cout << "y = " << wall.getPosition().y;
    wall.setPosition(wall.getPosition());
}

void Bird::apply_force(sf::Time& time) 
{
    auto t = time.asSeconds();
    velocity.y = G * t; 
    y = y + velocity.y * t / 2;
    bird.setPosition(x, y);
}

void Bird::fly() 
{
    velocity.y = -15;
    y = y + velocity.y * 1 / 2;
    bird.setPosition(x, y);
}

bool Wall::check() 
{
    auto x_coord = wall.getPosition().x + border;
    if(x_coord < 0 || x_coord > SIZE_X)
        return true;
    return false;
}

void draw_walls(std::vector<Wall>& walls, sf::RenderWindow& window) 
{
    if(walls.empty())
        return;
    for(auto& it : walls)
        draw_wall(it, window);
}

void move_walls(std::vector<Wall>& walls, sf::Time& time)
{
    if(walls.empty())
        return;
    for(auto& it : walls)
        it.move(time);
}

void delete_walls(std::vector<Wall>& walls)
{
    if(walls.empty())
        return;
    for(auto& p : walls) 
    {
        if(p.check())
        {
            std::cout << "hui";
            walls.pop_back();
        }
    }
}


int main() 
{
	sf::RenderWindow window(sf::VideoMode(SIZE_X, SIZE_Y), "My window");
	sf::Texture texture_bird;
    sf::Texture texture_wall;
    sf::Event event;
    sf::Clock clock;
    sf::Clock clock_wall;
    sf::Time time;
    sf::Time time_wall;
    std::vector<Wall> walls;
    texture_bird.loadFromFile("Bird-32.png");
    texture_wall.loadFromFile("images.png");
    auto bird = new Bird(SIZE_X/2, SIZE_Y/4, sf::Vector2f(0, 0), texture_bird);
    auto wall_up = new Wall(SIZE_X / 2, 0, sf::Vector2f(0, 0), texture_wall);
    auto wall_down = new Wall(SIZE_X / 2, 650, sf::Vector2f(0, 0), texture_wall);
    while(window.isOpen()) 
    {
        if(walls.empty())
        {
            walls.push_back(*wall_down);
            walls.push_back(*wall_up);
        }
        
		window.clear(sf::Color::Blue);
        draw_bird(*bird, window);
        draw_walls(walls, window);
        time = clock.getElapsedTime();
        time_wall = clock_wall.getElapsedTime();
        bird->apply_force(time);
        move_walls(walls, time_wall);
        while(window.pollEvent(event)) 
        {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if(event.key.code == sf::Keyboard::Space)
                    {
                        clock.restart();
                        bird->fly();
                    }
                    break;
                default:
                    break;
            }
        }
    delete_walls(walls);
    window.display();
    }
	return 0;
}
