#include<SFML/Graphics.hpp>
#include<iostream>
#include<math.h>
#include<cstdio>
#include<cstdlib>
#include<list>
#include<dlib/mlp.h>

#define SIZE_X 1000
#define SIZE_Y 1000
#define G      2000 / 1.5
#define border 50
#define speed  0.01 * 1.5
#define MAX_VEL 300 
#define BIRD_ACCEL -1500 / 1.5
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
        float distance_to_gap;
        float accel;
        dlib::mlp::kernel_1a_c* net;
        bool is_dead = false;

    public:
        Bird();
        Bird(const sf::Vector2f& vel, const sf::Texture& texture, const sf::Vector2f scale, 
             const sf::Vector2f position, const sf::Vector2f origin) : Object(vel, texture, scale, position, origin)
        {
            accel = 0;
            net = new dlib::mlp::kernel_1a_c(2, 6);
        };
        ~Bird(){};
        
        bool flag;
        void fly();
        void apply_force(sf::Time& time);
        void distance(sf::Time& time, Object& object);
        float get_passed_distance() { return passed_distance; }
        void is_collision(Object& point);
        float train(Object& object);
        bool get_is_dead() { return is_dead; }
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
	velocity.y += G * t + accel;
	object.move(0, velocity.y * t / 20000);
        accel = 0;
}

void Bird::fly()
{
	accel = BIRD_ACCEL;
	//object.move(0, velocity.y * 1 / 2);
}

void Bird::is_collision(Object& point)
{
    auto bird_pos = get_position();
    auto bird_origin = get_origin();

    auto point_pos = point.get_object().getPosition();

    // Столкновения с землей и вылет за карту
    if(bird_pos.y >= 900 || bird_pos.y <= 0)
    {
        is_dead = true;
        std::cout << "out of the map" << std::endl;
    }
    
    if(point_pos.x - 54.5 <= bird_pos.x + bird_origin.x &&
       point_pos.x + 54.5 >= bird_pos.x - bird_origin.x) 
    {
        if(bird_pos.y - bird_origin.y <= point_pos.y - 50 ||
           bird_pos.y + bird_origin.y >= point_pos.y + 50)
        {
            is_dead = true;
            std::cout << "collision" << std::endl;
        }
    }
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

// create our trees and points
void trees_and_targets(std::vector<Tree>& trees, std::vector<Object>& points, std::vector<sf::Texture>& textures)
{
    auto scale_up = myrandom(0.5, 1.5);
    auto scale_down = (900 - 100 - scale_up * 400) / 400;
    
    if (trees.empty() || trees.front().get_object().getPosition().x < 700)
    {
        // create trees;
        auto tree_up = std::make_unique<Tree>(sf::Vector2f(-0.05, 0), textures[0], sf::Vector2f(1, scale_up),
                                sf::Vector2f(SIZE_X + border, 0), sf::Vector2f(54.5, 0));
        auto tree_down = std::make_unique<Tree>(sf::Vector2f(-0.05, 0), textures[1], sf::Vector2f(1, scale_down),
                                  sf::Vector2f(SIZE_X + border, 900), sf::Vector2f(54.5, 400));
        auto point = std::make_unique<Object>(sf::Vector2f(-0.05, 0), textures[2], sf::Vector2f(1, 1),
                                sf::Vector2f(SIZE_X + border, real_size_y * scale_up + 50), sf::Vector2f(5, 5));
        // push them into vector
        trees.insert(trees.begin(), *tree_up);
        trees.insert(trees.begin(), *tree_down);
        points.insert(points.begin(), *point); 
    }
}

// create ten birds
void first_birds(std::list<Bird>& birds, sf::Texture& texture_bird)
{
    int count = 0;
    for(count = 0; count < 10; count++)
    {
        auto bird = std::make_unique<Bird>(sf::Vector2f(0, 0), texture_bird, sf::Vector2f(1, 1),
                    sf::Vector2f(SIZE_X / 2, 900 - 50 * (count + 1)), sf::Vector2f(16, 16));
        birds.push_back(*bird);
    }
}

void apply_force_birds(std::list<Bird>& birds, sf::Time& time)
{
    if(birds.empty())
        return;
    for(auto& it : birds)
        it.apply_force(time);
}

void fly_birds(std::list<Bird>& birds)
{
    if(birds.empty())
        return;
    for(auto& it : birds)
        it.fly();
}

void draw_birds(std::list<Bird>& birds, sf::RenderWindow& window)
{
    if(birds.empty())
        return;
    for(auto& it : birds)
        it.draw(window);
}

void check_collisions(std::list<Bird>& birds, Object& point)
{
    for(auto& p : birds)
        p.is_collision(point);
}

bool dead(Bird& bird)
{
    if(bird.get_is_dead())
        return true;
    return false;
}

float Bird::train(Object& point)
{
    dlib::matrix<double, 2, 1> sample;
    sample(0) = get_object().getPosition().x - point.get_object().getPosition().x - 54.5;
    sample(1) = get_object().getPosition().y - point.get_object().getPosition().y;
    float probability = (*net)(sample);
    std::cout << "proba = " << probability << std::endl;
    return probability;
}

void delete_birds(std::list<Bird>& birds)
{
    birds.remove_if(dead);
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

void Gameplay(sf::RenderWindow& window, sf::Clock& clock, sf::Time& time, std::list<Bird>& birds, std::vector<Object>& points)
{
    sf::Event event;
    /* if(static_cast<float>(time.asSeconds()) > 1.0)
    {
         for(auto& bird : birds)
         {
             if(bird.train(points.back()) > 0.5)
             {
                 std::cout << "here\n";
                 bird.fly();
                 clock.restart();
             }
         }
    }*/
    while (window.pollEvent(event))
    {
        switch (event.type) 
        {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Space)
                {
                    for(auto& bird : birds)
                    {
                        if(!bird.flag && (bird.get_velocity().y) > - MAX_VEL)
                        {
                            bird.flag = 1;
                            clock.restart();
                            fly_birds(birds);
                         }
                     }
                }
                break;
            case sf::Event::KeyReleased:
                for(auto& bird : birds)
                    if(event.key.code == sf::Keyboard::Space) bird.flag = 0;
                if(event.key.code == sf::Keyboard::M)
                {
                    DrawMenu(window);
                    clock.restart();
                }
            break;
            default:
                break;
        }
    }
}

bool StartGame() // just reset main() to this func
{
    int iter = 0;
    sf::RenderWindow window(sf::VideoMode(SIZE_X, SIZE_Y), "My window");
    sf::Texture texture_bird;
    std::vector<sf::Texture> barriers_textures;
    barriers_textures.resize(3);
    sf::Sprite background;
    sf::Texture backtext;
    MovableBackground bg;
    sf::Clock clock;
    sf::Clock clock_pisos;
    sf::Time time_pisos;
    sf::Time time;
    sf::Clock clock_game;
    sf::Time time_game;
    std::vector<Tree> trees;
    std::list<Bird> birds;
    std::list<Bird> best_birds;
    std::vector<Object> points;
    texture_bird.loadFromFile("Bird-32.png");
    barriers_textures[0].loadFromFile("tree_up.png");
    barriers_textures[1].loadFromFile("tree_down.png");
    barriers_textures[2].loadFromFile("img_target.png");
    sf::View view2;
    view2.setCenter(sf::Vector2f(500, 500));
    view2.setSize(sf::Vector2f(1000, 1000));
    first_birds(birds, texture_bird);
    while(window.isOpen())
    {
        trees_and_targets(trees, points, barriers_textures);
		
        bg.Render(window);
        bg.Update(window, speed * 10);
    
        
        draw_objects(trees, window);
        draw_objects(points, window);
        draw_birds(birds, window);
        time = clock.getElapsedTime();
        time_pisos = clock_game.getElapsedTime();        
        clock_pisos.restart();
         
        apply_force_birds(birds, time_pisos);
        move_objects(trees);
        move_objects(points);
        Gameplay(window, clock, time, birds, points);
        if(birds.size() == 4)
            std::copy(birds.begin(), birds.end(), best_birds.begin());
        if (birds.empty())
        {
            return DrawMenu(window); 
        }
 
        delete_objects(trees, 0);
        delete_objects(points, 548);
        check_collisions(birds, points.back());
        delete_birds(birds);
        window.setView(view2);
        window.display();
    }
    return 0;
}

void GameRunning() // if startgame==1 restart from very beginning, so function will kill old window 
{
	if (StartGame() > 0)
		GameRunning();
}

// differential evolution part
void Bird::distance(sf::Time& time, Object& closest_point)
{
    auto t = time.asSeconds();
    passed_distance = t * speed;
    distance_to_gap = closest_point.get_object().getPosition().x - get_object().getPosition().x +
                      closest_point.get_object().getPosition().y - get_object().getPosition().y;
}

bool compare(Bird& bird_1, Bird& bird_2)
{    
    return bird_1.get_passed_distance() > bird_2.get_passed_distance();
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
