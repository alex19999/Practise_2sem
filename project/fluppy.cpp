#include<SFML/Graphics.hpp>
#include<iostream>
#include<math.h>
#include<cstdio>
#include<cstdlib>
#include<list>

#define SIZE_X 1000
#define SIZE_Y 1000
#define G      0.08
#define border 50
#define speed  0.01

#define real_size_x 109
#define real_size_y 400

class Object
{
    protected:
        sf::Vector2f velocity;
        sf::Sprite object;

    public:
        // c-tor and d-tor
        Object();
        Object(const sf::Vector2f& vel, const sf::Texture& texture, const sf::Vector2f scale, 
               const sf::Vector2f position, const sf::Vector2f origin) : 
               velocity(vel), object(texture) 
        {
            object.setOrigin(origin.x, origin.y);
            object.scale(scale.x, scale.y);
            object.setPosition(position.x, position.y);
        };
        ~Object(){};

        // methods
        sf::Vector2f get_velocity() { return velocity; }
        sf::Sprite get_object()     { return object;   }
        sf::Vector2f get_position() { return object.getPosition(); }
        sf::Vector2f get_scale()    { return object.getScale();    }
        sf::Vector2f get_origin()   { return object.getOrigin();   }
        void draw(sf::RenderWindow& window);
};

class Tree : public Object
{
    public:
        Tree();
        Tree(const sf::Vector2f& vel, const sf::Texture& texture, sf::Vector2f scale, 
             const sf::Vector2f position , const sf::Vector2f origin) : 
             Object(vel, texture, scale, position, origin)
        {};
        ~Tree(){};
        
        void move();
        bool check();
};

class Bird : public Object
{
    public:
        Bird();
        Bird(const sf::Vector2f& vel, const sf::Texture& texture, const sf::Vector2f scale, 
             const sf::Vector2f position, const sf::Vector2f origin) : 
             Object(vel, texture, scale, position, origin)
        {};
        ~Bird(){};
        
        void fly();
        void apply_force(sf::Time& time);
        bool is_collision(std::vector<Tree>& trees);
};


class MovableBackground 
{
    public:
	    MovableBackground();
	    void Update(sf::RenderWindow &window, float elapsedTime);
	    void Render(sf::RenderWindow &window);

    private:
	    sf::Texture bgTex;
	    sf::Texture bg2Tex;

	    sf::RectangleShape bgShape;
	    sf::Vector2f bgSize;

	    sf::Sprite bgSprite;
	    sf::Sprite bg2Sprite;

	    float bgSpeed;
	    float bgX;
	    float bg2X;
	    float windowbg1diff;
};

MovableBackground::MovableBackground() 
{
	bgSpeed = 0.03;

	bgTex.loadFromFile("background.jpg");
	bgTex.setSmooth(false);
	bgTex.setRepeated(true);

	bgX = bgShape.getPosition().y;
	bgSize.x = SIZE_X*1.5;
	bgSize.y = SIZE_Y;

	bgShape.setTexture(&bgTex);
	bgShape.setTextureRect(sf::IntRect(0, 0, SIZE_X*1.5, SIZE_Y));
	bgShape.setSize(bgSize);
}

void MovableBackground::Update(sf::RenderWindow &window, float elapsedTime) 
{
	if (bgX < SIZE_X)
	{
		bgX =(bgX + bgSpeed * elapsedTime);
		if (bgX >= (SIZE_X / 2)) bgX = 0;
	}
	else
	{
		bgX = 0;
	}
	bgShape.setPosition(-bgX, 0);
}

void MovableBackground::Render(sf::RenderWindow &window)
{
	window.draw(bgShape);
}

void Object::draw(sf::RenderWindow& window) 
{
    window.draw(this->get_object());
}

void Tree::move() 
{
    object.move(sf::Vector2f(velocity.x, 0));
}

void Bird::apply_force(sf::Time& time) 
{
    auto t = time.asSeconds();
    velocity.y = G * t; 
    object.move(0, velocity.y * t / 2);
}

void Bird::fly() 
{
    velocity.y = -15;
    object.move(0, velocity.y * 1 / 2);
}

bool Bird::is_collision(std::vector<Tree>& trees)
{
    auto bird_pos = get_position();
    auto bird_origin = get_origin();

    //std::cout << "bird_x = " << bird_pos.x << " bird_y = " << bird_pos.y << std::endl;

    for(auto& it : trees) 
    {
        auto tree_pos = it.get_position();
        auto tree_origin = it.get_origin();
        auto tree_scale = it.get_scale();

        //std::cout << "tree_x = " << tree_pos.x << " tree_y = " << tree_pos.y << std::endl;

        if(bird_pos.y + bird_origin.y <= real_size_y * tree_scale.y + tree_origin.y)
        {
           // std::cout << "hui\n";
            if(bird_pos.x + bird_origin.x == static_cast<int>(tree_pos.x))
            {
                std::cout << "collision_up\n";
                return 1;
            }
        } 
        
        if(bird_pos.y + bird_origin.y <= 900 - real_size_y * tree_scale.y + tree_origin.y)
        {
            if(bird_pos.x + bird_origin.x == static_cast<int>(tree_pos.x))
            {
                std::cout << "collision_down\n";
                return 1;
            }
        }
        return 0;
    }
}

bool Tree::check() 
{
    auto x_coord = object.getPosition().x + border;
    if(x_coord < 0 || x_coord > SIZE_X + border)
        return true;
    return false;
}

void draw_trees(std::vector<Tree>& trees, sf::RenderWindow& window) 
{
    if(trees.empty())
        return;
    for(auto& it : trees)
        it.draw(window);
}

void move_trees(std::vector<Tree>& trees)
{
    if(trees.empty())
        return;
    for(auto& it : trees)
        it.move();
}

void delete_trees(std::vector<Tree>& trees)
{
    if(trees.empty())
        return;
    for(auto& it : trees) 
    {
        if(it.check())
        {
            trees.pop_back();
        }
    }
}

float myrandom(float min, float max) 
{
    return (float)(rand())/RAND_MAX*(max - min) + min;
}

int main() 
{
    sf::RenderWindow window(sf::VideoMode(SIZE_X, SIZE_Y), "My window");
    sf::Texture texture_bird;
    sf::Texture texture_tree_up;
    sf::Texture texture_tree_down;
    sf::Sprite background;
    sf::Texture backtext;
    MovableBackground bg;
    sf::Event event;
    sf::Clock clock;
    sf::Time time;
    sf::Clock clock_game;
    sf::Time time_game;
    std::vector<Tree> trees;
    texture_bird.loadFromFile("Bird-32.png");
    texture_tree_up.loadFromFile("tree_up.png");
    texture_tree_down.loadFromFile("tree_down.png");
    sf::View view2;
    view2.setCenter(sf::Vector2f(500, 500));
    view2.setSize(sf::Vector2f(1000, 1000));

    auto bird = new Bird(sf::Vector2f(0, 0), texture_bird, sf::Vector2f(1, 1), 
                         sf::Vector2f(SIZE_X / 2, SIZE_Y / 2), sf::Vector2f(32, 32));
    while(window.isOpen()) 
    {
        auto scale_up = myrandom(0.5, 1.5);
        auto scale_down = (900 - 100 - scale_up * 400) / 400;
        auto tree_up = new Tree(sf::Vector2f(-0.05, 0), texture_tree_up, sf::Vector2f(1, scale_up), 
                                sf::Vector2f(SIZE_X, 0), sf::Vector2f(50, 0));
        auto tree_down = new Tree(sf::Vector2f(-0.05, 0), texture_tree_down, sf::Vector2f(1, scale_down), 
                                  sf::Vector2f(SIZE_X, 900), sf::Vector2f(50, 400));
        
        if(trees.empty()) // || trees.back().get_object().getPosition().x < 700)
        {
            trees.push_back(*tree_down);
            trees.push_back(*tree_up);
            std::cout << "size =" << trees.size() << "\n";
        }
        
        bg.Render(window); 
        bg.Update(window, speed * 10); 
        
        bird->draw(window);
        draw_trees(trees, window);
        time = clock.getElapsedTime();
        bird->apply_force(time);
        move_trees(trees);
        bird->is_collision(trees);
        while(window.pollEvent(event)) 
        {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if(event.key.code == sf::Keyboard::Space)
                    {
                        bird->fly();
                        clock.restart();
                    }
                    break;
                default:
                    break;
            }
        }
    delete_trees(trees);
    window.setView(view2);
    window.display();
    }
	return 0;
}
