#include<SFML/Graphics.hpp>
#include<iostream>
#include<math.h>
#include<cstdio>
#include<cstdlib>
#include<list>
#include<dlib/mlp.h>

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
        void move();
        bool check(unsigned int line);
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
};

class Bird : public Object
{
    private:
        float passed_distance;
        dlib::mlp::kernel_1a_c net;

    public:
        Bird();
        Bird(const sf::Vector2f& vel, const sf::Texture& texture, const sf::Vector2f scale, 
             const sf::Vector2f position, const sf::Vector2f origin) :
             net(2, 6), Object(vel, texture, scale, position, origin)
        {};
        ~Bird(){};
        
        void fly();
        void apply_force(sf::Time& time);
        void distance(sf::Time& time);
        float get_passed_distance() { return passed_distance; }
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

void Object::move() 
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

    // Столкновения с землей и вылет за карту
    if(bird_pos.y >= 900 || bird_pos.y <= 0)
    {
        std::cout << "out of the map" << std::endl;
        return 1;
    }

    for(auto& it : trees) 
    {
        auto tree_pos = it.get_position();
        auto tree_origin = it.get_origin();
        auto tree_scale = it.get_scale();

        if(tree_pos.x - tree_origin.x <= bird_pos.x + bird_origin.x &&
           tree_pos.x + tree_origin.x >= bird_pos.x - bird_origin.x)
        {
            // Столкновение либо с верхним деревом, либо с нижним соответственно
            if(bird_pos.y - bird_origin.y <= 800 - real_size_y * tree_scale.y ||
               bird_pos.y >= 900 - real_size_y * tree_scale.y)
            {
                std::cout << "collision" << std::endl;
                return 1;
            }

            return 0;
        }
    }
    return 0;
}

bool Object::check(unsigned int line) 
{
    auto x_coord = object.getPosition().x + border;
    if(x_coord < line || x_coord > SIZE_X + 2 * border)
        return true;
    return false;
}

template <typename T>
void draw_objects(std::vector<T>& objects, sf::RenderWindow& window) 
{
    if(objects.empty())
        return;
    for(auto& it : objects)
        it.draw(window);
}

template <typename T>
void move_objects(std::vector<T>& objects)
{
    if(objects.empty())
        return;
    for(auto& it : objects)
        it.move();
}

template <typename T>
void delete_objects(std::vector<T>& objects, unsigned int line)
{
    int iter = 0;
    
    if(objects.empty())
        return;
    
	while (iter < objects.size())
	{
        if (objects[iter].check(line))
		{
		    objects[iter] = objects.back();
			objects.pop_back();
		}
		
        else
			iter++;
    }
}

float myrandom(float min, float max) 
{
    return (float)(rand())/RAND_MAX*(max - min) + min;
}

void WindowInitialize(sf::RenderWindow& window) // window set
{
	window.setFramerateLimit(60);
}

void PlayGame() //clear
{

}

void ExitGame(sf::RenderWindow& window) //clear
{
	window.close();
}

bool ResetGame(sf::RenderWindow& window)
{
	return 1;
}

bool MouseCheck(sf::Sprite object, sf::Texture texture, sf::RenderWindow& window) // Pointer pos check
{
	return ((sf::Mouse::getPosition(window).x > object.getPosition().x) &&
		    (sf::Mouse::getPosition(window).x < object.getPosition().x + 
             texture.getSize().x * object.getScale().x) &&
		    (sf::Mouse::getPosition(window).y < object.getPosition().y + 
             texture.getSize().y * object.getScale().y) &&
		    (sf::Mouse::getPosition(window).y > object.getPosition().y));
}

void CreateTextButton(sf::Text& text, sf::Sprite object, sf::Texture texture, std::string s) //Text pos
{
	text.setString(s);
	text.setOrigin(text.getLocalBounds().width / 2, 
                   text.getLocalBounds().height - text.getLocalBounds().left - 12);
	text.setPosition(object.getPosition().x + texture.getSize().x / 2, 
                     object.getPosition().y + texture.getSize().y / 2);
}

int DrawMenu(sf::RenderWindow& window)
{

	sf::Texture mapTexture; // Background
	mapTexture.loadFromFile("menu_map.jpg");
	mapTexture.setSmooth(true);

	sf::Sprite background; // Background
	background.setTexture(mapTexture);
	background.setOrigin(mapTexture.getSize().x / 2, mapTexture.getSize().y / 2);
	background.setPosition(window.getSize().x / 2, window.getSize().y / 2);
	background.setScale(window.getSize().y * 1.0 / mapTexture.getSize().y,
		window.getSize().y * 1.0 / mapTexture.getSize().y);

	sf::Texture press_texture; //  swap if pointer
	press_texture.loadFromFile("button_press.png");

	sf::Texture texture; // Button texture
	texture.loadFromFile("button.png");

	sf::Sprite but_play(texture); // PLAY
	but_play.setPosition(window.getSize().x / 2 - texture.getSize().x / 2,
		window.getSize().y / 2 - 150);

	sf::Sprite but_exit(texture); // EXIT
	but_exit.setPosition(window.getSize().x / 2 - texture.getSize().x / 2,
		window.getSize().y / 2 + 30);

	sf::Sprite but_restart(texture); // RESTART
	but_restart.setPosition(window.getSize().x / 2 - texture.getSize().x / 2,
		window.getSize().y / 2 - 60);

	sf::Font font;// font
	font.loadFromFile("11774.ttf");

	sf::Text text_play("", font, 2 * texture.getSize().y / 3); // Create "PLAY"
	text_play.setColor(sf::Color::White);
	CreateTextButton(text_play, but_play, texture, "PLAY");

	sf::Text text_exit("", font, 2 * texture.getSize().y / 3); // Create "EXIT"
	text_exit.setColor(sf::Color::White);
	CreateTextButton(text_exit, but_exit, texture, "EXIT");

	sf::Text text_restart("", font, 2 * texture.getSize().y / 3); // Create "RESTART"
	text_restart.setColor(sf::Color::White);
	CreateTextButton(text_restart, but_restart, texture, "RESTART");

	while (window.isOpen())
	{

		sf::Event event;
		window.clear(sf::Color(100, 100, 100));
		window.draw(background);
		window.draw(but_exit);
		window.draw(but_play);
		window.draw(but_restart);
		window.draw(text_play);
		window.draw(text_exit);
		window.draw(text_restart);
		but_exit.setTexture(texture);
		but_play.setTexture(texture);
		but_restart.setTexture(texture);

		if (MouseCheck(but_exit, press_texture, window)) // Mouse pos check
		{
			but_exit.setTexture(press_texture);
		}

		if (MouseCheck(but_play, press_texture, window))
		{
			but_play.setTexture(press_texture);
		}

		if (MouseCheck(but_restart, press_texture, window))
		{
			but_restart.setTexture(press_texture);
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) // LMB check
		{
			if (MouseCheck(but_play, press_texture, window))
			{
				PlayGame();
				return -1; // magic
			}

			if (MouseCheck(but_exit, press_texture, window))
			{
				ExitGame(window);
				break;
			}

			if (MouseCheck(but_restart, press_texture, window))
			{
				ResetGame(window);
				return 1;
			}

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) // Close game by Esc
		{
			ExitGame(window);
			break;
		}

		while (window.pollEvent(event)) // Close window by X
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}

		window.display();
	}

}

bool StartGame() // just reset main() to this func
{
    int iter = 0;
	sf::RenderWindow window(sf::VideoMode(SIZE_X, SIZE_Y), "My window");
	sf::Texture texture_bird;
	sf::Texture texture_tree_up;
	sf::Texture texture_tree_down;
    sf::Texture texture_point;
	sf::Sprite background;
	sf::Texture backtext;
	MovableBackground bg;
	sf::Event event;
	sf::Clock clock;
	sf::Time time;
	sf::Clock clock_game;
	sf::Time time_game;
	std::vector<Tree> trees;
    std::vector<Object> points;
    std::vector<float> distance_x;
    std::vector<float> distance_y;
	texture_bird.loadFromFile("Bird-32.png");
	texture_tree_up.loadFromFile("tree_up.png");
	texture_tree_down.loadFromFile("tree_down.png");
    texture_point.loadFromFile("img_target.png");
	sf::View view2;
	view2.setCenter(sf::Vector2f(500, 500));
	view2.setSize(sf::Vector2f(1000, 1000));

	auto bird = new Bird(sf::Vector2f(0, 0), texture_bird, sf::Vector2f(1, 1),
		                 sf::Vector2f(SIZE_X / 2, SIZE_Y / 2), sf::Vector2f(16, 16));
	while (window.isOpen())
	{
		auto scale_up = myrandom(0.5, 1.5);
		auto scale_down = (900 - 100 - scale_up * 400) / 400;
		
        if (trees.empty() || trees.front().get_object().getPosition().x < 700)
		{
            auto tree_up = new Tree(sf::Vector2f(-0.1, 0), texture_tree_up, sf::Vector2f(1, scale_up),
                                    sf::Vector2f(SIZE_X + border, 0), sf::Vector2f(54.5, 0));
            auto tree_down = new Tree(sf::Vector2f(-0.1, 0), texture_tree_down, sf::Vector2f(1, scale_down), 
                                      sf::Vector2f(SIZE_X + border, 900), sf::Vector2f(54.5, 400));
            auto point = new Object(sf::Vector2f(-0.1, 0), texture_point, sf::Vector2f(1, 1),
                                    sf::Vector2f(tree_up->get_object().getPosition().x, real_size_y * scale_up + 50),
                                    sf::Vector2f(5, 5));   
            auto it = trees.begin();
            trees.insert(it, *tree_up);
            it = trees.begin();
            trees.insert(it, *tree_down);
            auto begin = points.begin();
            points.insert(begin, *point);
		}
		
        bg.Render(window);
		bg.Update(window, speed * 10);

		bird->draw(window);
		draw_objects(trees, window);
        draw_objects(points, window);
		time = clock.getElapsedTime();
        time_game = clock_game.getElapsedTime();
		bird->apply_force(time);
		move_objects(trees);
        move_objects(points);

		if (bird->is_collision(trees))
		{
			return DrawMenu(window);
		}

		while (window.pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Space)
				{
					clock.restart();
                    bird->fly();
				}
				break;
			case sf::Event::KeyReleased:
				if (event.key.code == sf::Keyboard::M)
				{
					DrawMenu(window);
					clock.restart();
				}
				break;
			default:
				break;
			}
		}
		delete_objects(trees, 0);
        delete_objects(points, 548);
		window.setView(view2);
		window.display();
	}
	return 0;
}

void GameRunning() // if startgame==1 restart from very beginning, so function will kill old window 
{
	if (StartGame() > 0)
	{
		GameRunning();
	}
}

// differential evolution part
void Bird::distance(sf::Time time)
{
    auto t = time.asSeconds();
    passed_distance = t * speed;
}

bool compare(const Bird& bird_1, const Bird& bird_2)
{    
    return bird_1.distance() > bird_2.distance();
}   

int main()
{
	GameRunning();
	return 0;
}

/*differential evolution

void Bird::distance(sf::Time time)
{
    auto t = time.asSeconds()
    passed_distance = t * speed;
}

bool compare(const Bird& bird_1, const Bird& bird_2)
{
    return bird_1.distance() > bird_2.distance();
}


std::vector<Bird> generation()
{
    // Генерируем новую популяцию;
};

std::vector<Bird> selection(const std::vector<Bird>& birds)
{
    // Отбираем лучшие варианты;
    std::vector<Bird> best_birds;
    std::sort(birds.begin(), birds.end(), compare);

    return best_birds;
}

Bird skreschevanie(three birds)
{
    // Разность от двух и движемся в направлении третьей;
}

Bird mutation(two birds)
{
    // Берем две птицы и получаем новую
}

 Нейрочасть */
