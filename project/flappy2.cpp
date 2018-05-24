#include<SFML/Graphics.hpp>
#include<iostream>
#include<math.h>
#include<cstdio>
#include<cstdlib>
#include<list>

#define SIZE_X 1000
#define SIZE_Y 1000
#define G      2000 / 1.5
#define border 50
#define GAP    100
#define speed  0.01 * 1.5
#define MAX_VEL 300
#define BIRD_ACCEL -1500/10
#define BIRD_NUM 10

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
	~Object() {};

	// methods
	sf::Vector2f get_velocity() const     { return velocity;             }
	sf::Sprite get_object()     const     { return object;               }
	sf::Vector2f get_position() const     { return object.getPosition(); }
	sf::Vector2f get_scale()    const     { return object.getScale();    }
	sf::Vector2f get_origin()   const     { return object.getOrigin();   }
    void set_position(const sf::Vector2f position) { object.setPosition(position); }
	void draw(sf::RenderWindow& window);
    void move();
    bool check(unsigned int line);
};

class Tree : public Object
{
public:
	Tree();
	Tree(const sf::Vector2f& vel, const sf::Texture& texture, sf::Vector2f scale,
		 const sf::Vector2f position, const sf::Vector2f origin) :
		 Object(vel, texture, scale, position, origin)
	{};
	~Tree() {};

};

class Bird : public Object
{
private:
    float passed_distance;
    float distance_to_gap_x;
    float distance_to_gap_y;
    float accel;
    bool is_dead = false;
    float weight_x = 0.01;
    float weight_y = 0.01;
    float weight_vel = 0.01;
    bool ready_to_fly = false;
public:
	Bird();
	Bird(const sf::Vector2f& vel, const sf::Texture& texture, const sf::Vector2f scale,
		 const sf::Vector2f position, const sf::Vector2f origin, const sf::Vector3f weight) :
		 Object(vel, texture, scale, position, origin)
	{
		accel = 0;
        weight_x = weight.x;
        weight_y = weight.y;
        weight_vel = weight.z;
	};
    Bird(const Bird& bird): Object(bird.get_velocity(), *bird.get_object().getTexture(), 
                                   bird.get_scale(), bird.get_position(), bird.get_origin())
    {
        accel = 0;
        passed_distance = bird.get_passed_distance();
        weight_x = bird.get_weight_x();
        weight_y = bird.get_weight_y();
        weight_vel = bird.get_weight_vel();
    };

	~Bird() {};
    
    Bird operator-(const Bird& other) const;
    Bird operator+(const Bird& other) const;
	bool flag;
	void fly();
    void set_flag(bool flag_) { flag = flag_; } 
	void apply_force(sf::Time& time);
	void set_passed_distance(float distance) { passed_distance = distance; }
    void distance_to_gap(Object& point);
    float get_distance_to_gap_x() const { return distance_to_gap_x; }
    float get_distance_to_gap_y() const { return distance_to_gap_y; }
	float get_passed_distance()   const { return passed_distance;   }
    float get_weight_x()          const { return weight_x;          }
    float get_weight_y()          const { return weight_y;          }
    float get_weight_vel()        const { return weight_vel;        }
	void is_collision(Object& point);
    bool get_is_dead() const { return is_dead; }
    void predict();
    bool is_fly() const { return ready_to_fly; }
    void set_weight_x(float w_x) { weight_x = w_x; }
    void set_weight_y(float w_y) { weight_y = w_y; }
    void set_weight_vel(float w_vel) { weight_vel = w_vel; }
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
	bgSize.x = SIZE_X * 1.5;
	bgSize.y = SIZE_Y;

	bgShape.setTexture(&bgTex);
	bgShape.setTextureRect(sf::IntRect(0, 0, SIZE_X*1.5, SIZE_Y));
	bgShape.setSize(bgSize);
}

void MovableBackground::Update(sf::RenderWindow &window, float elapsedTime)
{
	if (bgX < SIZE_X)
	{
		bgX = (bgX + bgSpeed * elapsedTime);
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
	object.move(0, velocity.y * t / 2);
	accel = 0;
}

void Bird::fly()
{
	accel = BIRD_ACCEL;
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
    
    if(point_pos.x - real_size_x / 2 <= bird_pos.x + bird_origin.x &&
       point_pos.x + real_size_x / 2 >= bird_pos.x - bird_origin.x) 
    {
        if(bird_pos.y - bird_origin.y <= point_pos.y - border ||
           bird_pos.y + bird_origin.y >= point_pos.y + border)
        {
            is_dead = true;
        }
    }
}

bool Object::check(unsigned int line)
{
	auto x_coord = object.getPosition().x + border;
	if (x_coord < line || x_coord > SIZE_X + 2 * border)
		return true;
	return false;
}

void Bird::distance_to_gap(Object& point)
{
    distance_to_gap_x = point.get_position().x - point.get_origin().x - 
                        get_position().x + get_origin().x;
    distance_to_gap_y = point.get_position().y - get_position().y;
}


template <typename T> 
void draw_objects(std::vector<T>& objects, sf::RenderWindow& window)
{
	if (objects.empty())
		return;
	for (auto& it : objects)
		it.draw(window);
}

template <typename T> 
void move_objects(std::vector<T>& objects)
{
	if (objects.empty())
		return;
	for (auto& it : objects)
		it.move();
}

template <typename T>
void delete_objects(std::vector<T>& objects, unsigned int line)
{
	int iter = 0;

	if (objects.empty())
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
	return (float)(rand()) / RAND_MAX * (max - min) + min;
}

// create our trees and points
void trees_and_targets(std::vector<Tree>& trees, std::vector<Object>& points, std::vector<sf::Texture>& textures)
{
    auto scale_up = myrandom(0.5, 1.5);
    auto scale_down = (900 - GAP - scale_up * real_size_y) / real_size_y;
    
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
    for(count = 0; count < BIRD_NUM; count++)
    {
        auto bird = std::make_unique<Bird>(sf::Vector2f(0, 0), texture_bird, sf::Vector2f(1, 1),
                    sf::Vector2f(SIZE_X / 2, 900 - 50 * (count + 1)), sf::Vector2f(16, 16),
                    sf::Vector3f(myrandom(0, 1), myrandom(0, 1), myrandom(0, 1)));
        birds.push_back(*bird);
    }
}

void set_flags_birds(std::list<Bird>& birds, bool flag)
{
    if(birds.empty())
        return;
    for(auto& it : birds)
        it.set_flag(flag);
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

void delete_birds(std::list<Bird>& birds, std::vector<Bird>& new_birds)
{
    if(birds.size() <= 4)
        for(auto& bird : birds)
            if(bird.get_is_dead())
                new_birds.insert(new_birds.begin(), bird);
        
    birds.remove_if(dead);
}

void distance_to_gap_birds(std::list<Bird>& birds, Object& point)
{
    if(birds.empty())
        return;
    for(auto& it : birds)
        it.distance_to_gap(point);
}

void Bird::predict()
{
    auto weight_sum = distance_to_gap_x * weight_x + distance_to_gap_y * weight_y 
                      + get_velocity().y * weight_vel;
    auto predict =  1 / (1 + exp(weight_sum));

    if(predict > 0.3)
        ready_to_fly = true;
    else
        ready_to_fly = false;
}

void predict(std::list<Bird>& birds)
{
    if(birds.empty())
        return;
    for(auto& it : birds)
        it.predict();
}

void show_weights(std::list<Bird>& birds)
{
    if(birds.empty())
        return;
    for(auto& bird : birds)
    {
        std::cout << "weight_x = " << bird.get_weight_x() << std::endl;
        std::cout << "weight_y = " << bird.get_weight_y() << std::endl;
        std::cout << "weight_vel = " << bird.get_weight_vel() << std::endl;
    }
}

void self_fly_birds(std::list<Bird>& birds)
{
    if(birds.empty())
        return;
    for(auto& it : birds)
        if(it.is_fly())
            it.fly();
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
		(sf::Mouse::getPosition(window).x < object.getPosition().x + texture.getSize().x) &&
		(sf::Mouse::getPosition(window).y < object.getPosition().y + texture.getSize().y) &&
		(sf::Mouse::getPosition(window).y > object.getPosition().y));
	// did some but nothing
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

//differential evolution
void distance_birds(std::list<Bird>& birds, sf::Time& time)
{
    if(birds.empty())
        return;
    auto t = time.asSeconds();
    for(auto& it : birds)
        it.set_passed_distance(t * speed);
}

bool compare(Bird& bird_1, Bird& bird_2)
{
    return bird_1.get_passed_distance() > bird_2.get_passed_distance();
}

Bird Bird::operator-(const Bird& other) const 
{
    auto bird(other);
    bird.set_weight_x(weight_x - other.get_weight_x());
    bird.set_weight_y(weight_y - other.get_weight_y());
    bird.set_weight_vel(weight_vel - other.get_weight_vel());
    return bird;
}

Bird Bird::operator+(const Bird& other) const
{
    auto bird(other);
    bird.set_weight_x(weight_x + other.get_weight_x());
    bird.set_weight_y(weight_y + other.get_weight_y());
    bird.set_weight_vel(weight_vel + other.get_weight_vel());
    return bird;
}

void new_generation(std::vector<Bird>& best_birds, std::list<Bird>& birds)
{
    // generate new birds
    best_birds.push_back(best_birds[2] + (best_birds[0] - best_birds[1]));
    best_birds.push_back(best_birds[3] + (best_birds[0] - best_birds[1]));
    best_birds.push_back(best_birds[0] + (best_birds[1] - best_birds[2]));
    best_birds.push_back(best_birds[3] + (best_birds[1] - best_birds[2]));
    best_birds.push_back(best_birds[0] + (best_birds[2] - best_birds[3]));
    best_birds.push_back(best_birds[2] + (best_birds[3] - best_birds[0]));
    
    for(auto& it : best_birds)
        birds.push_back(it);
    std::cout << "size = " << birds.size() << std::endl;
}

void restart_game(std::list<Bird>& birds)
{
    if(birds.empty())
        return;
    for(auto& bird : birds)
        bird.set_position(sf::Vector2f(SIZE_X / 2, SIZE_Y / 2));
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
	sf::Event event;
	sf::Clock clock_game;
	sf::Time time_game;
	sf::Clock clock;
	sf::Clock clock_pisos;
	sf::Time time;
	sf::Time time_pisos;
	std::vector<Tree> trees;
    std::list<Bird> birds;
    std::vector<Bird> best_birds;
    std::vector<Object> points;
    std::vector<float> distance;
	texture_bird.loadFromFile("Bird-32.png");
	barriers_textures[0].loadFromFile("tree_up.png");
    barriers_textures[1].loadFromFile("tree_down.png");
    barriers_textures[2].loadFromFile("img_target.png");
	sf::View view2;
	view2.setCenter(sf::Vector2f(SIZE_X / 2, SIZE_Y / 2));
	view2.setSize(sf::Vector2f(SIZE_X, SIZE_Y));
    first_birds(birds, texture_bird);
	while (window.isOpen())
	{
        trees_and_targets(trees, points, barriers_textures);
		bg.Render(window);
		bg.Update(window, speed * 10);

		draw_birds(birds, window);
		draw_objects(trees, window);
        draw_objects(points, window);
		time = clock.getElapsedTime();
		time_pisos = clock_pisos.getElapsedTime();
		clock_pisos.restart();

		time_game = clock_game.getElapsedTime();
		apply_force_birds(birds, time_pisos);
		move_objects(trees);
        move_objects(points);

        if(static_cast<int>(time_game.asSeconds()) % 2 == 0)
        {
            distance_to_gap_birds(birds, points.back());
            predict(birds);
            self_fly_birds(birds);
            
        }
        
        if (birds.empty())
        {
            new_generation(best_birds, birds);
            restart_game(birds);
            trees.clear();
            points.clear();
            best_birds.clear();
            clock_game.restart();
            show_weights(birds);
            //return DrawMenu(window); 
        }
        
		while (window.pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:

				if (event.key.code == sf::Keyboard::Space) //&& !bird->flag && (bird->get_velocity().y)>-MAX_VEL)
				{
					set_flags_birds(birds, 1);
					clock.restart();
					fly_birds(birds);
				}

				break;
			case sf::Event::KeyReleased:
				if (event.key.code == sf::Keyboard::Space) set_flags_birds(birds, 0);
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
        distance_birds(birds, time_game);
		delete_objects(trees, 0);
        delete_objects(points, 548);
        check_collisions(birds, points.back());
        delete_birds(birds, best_birds);
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

int main()
{
	GameRunning();
	return 0;
}
/*
differential evolution
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
// Ãåíåðèðóåì íîâóþ ïîïóëÿöèþ;
};
std::vector<Bird> selection(const std::vector<Bird>& birds)
{
// Îòáèðàåì ëó÷øèå âàðèàíòû;
std::vector<Bird> best_birds;
std::sort(birds.begin(), birds.end(), compare);
return best_birds;
}
Bird skreschevanie(three birds)
{
// Ðàçíîñòü îò äâóõ è äâèæåìñÿ â íàïðàâëåíèè òðåòüåé;
}
Bird mutation(two birds)
{
// Áåðåì äâå ïòèöû è ïîëó÷àåì íîâóþ
}
*/
