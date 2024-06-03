#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <vector> 
#include <memory>

class Shape {
	std::shared_ptr<sf::Shape> m_shape;
	std::string m_text = "";
	sf::Vector2f m_vel;
	sf::Vector2f m_viewSize;

public:

	Shape() {}

	Shape(std::shared_ptr<sf::Shape> shape, std::string text, sf::Vector2f& vel, sf::Vector2f& viewSize)
		: m_shape(shape)
		, m_text(text)
		, m_vel(vel)
		, m_viewSize(viewSize) {}

	void updatePos()
	{
		std::shared_ptr<sf::Shape> newShape = getShape();
		sf::Vector2f currentPosition = newShape->getPosition();

		sf::Vector2f updatedPostion = newShape->getPosition();

		if (updatedPostion.x < 0 || updatedPostion.x > m_viewSize.x - newShape->getLocalBounds().width) {
			m_vel.x *= -1;
		}
		if (updatedPostion.y < 0 || updatedPostion.y > m_viewSize.y - newShape->getLocalBounds().width) {
			m_vel.y *= -1;
		}

		newShape->setPosition(currentPosition.x + m_vel.x, currentPosition.y + m_vel.y);
	}

	std::shared_ptr<sf::Shape> getShape()
	{
		return m_shape;
	}

	std::string& getText()
	{
		return m_text;
	}

};

class Sence
{
	sf::Vector2f			m_viewSize;
	sf::RenderWindow		m_window;
	sf::Font				m_font;
	sf::Text				m_text;
	std::vector<Shape>		m_shapes;
	int						m_fontSize = 18;

private:
	Shape m_shape;

public:
	void loadFromFile(const std::string& filepath)
	{
		std::ifstream fin(filepath);
		std::string type;
		std::string text = "";
		std::string fontFace;
		sf::Vector2f size;
		sf::Vector2f pos;
		sf::Vector2f vel;
		unsigned int r, g, b;
		float radius = 0.f;
		int fontSize = 18;

		if (!fin.is_open())
		{
			std::cerr << "Unable to read the sources file!" << "\n";
			exit(-1);
		}

		while (fin >> type)
		{
			if (type == "Window")
			{
				fin >> size.x >> size.y;
				createWindow(size);
			}
			if (type == "Font")
			{
				fin >> fontFace >> fontSize >> r >> g >> b;
				setFont(fontFace, fontSize, r, g, b);
			}
			if (type == "Circle")
			{
				fin >> text >> pos.x >> pos.y >> vel.x >> vel.y >> r >> g >> b >> radius;
				add_shape(pos, vel, r, g, b, radius, text);
			}
			if (type == "Rectangle")
			{
				fin >> text >> pos.x >> pos.y >> vel.x >> vel.y >> r >> g >> b >> size.x >> size.y;
				add_shape(pos, vel, r, g, b, size, text);
			}

		}
	}

	void Run()
	{
		std::cout << "Running Program!" << "\n";

		sf::Text text;
		text.setFont(m_font);
		text.setString("Hello world");
		
		m_window.setFramerateLimit(60);

		while (m_window.isOpen())
		{
			sf::Event event;
			while (m_window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					m_window.close();
				}
			}
			m_window.clear();

			for (auto& shape : m_shapes)
			{
				shape.updatePos();
				sf::Text stext;
				stext.setString(shape.getText());
				m_window.draw(*shape.getShape());
				m_window.draw(stext);
			}
			m_window.display();
		}
	}

	void createWindow(const sf::Vector2f& vs)
	{
		m_viewSize = vs;
		m_window.create(sf::VideoMode(vs.x, vs.y), "Learn SFML!");
	}

	void setFont(const std::string& path, int size, sf::Uint8 r, sf::Uint8 g, sf::Uint8 b)
	{
		if (!m_font.loadFromFile(path))
		{
			std::cerr << "Unable to load the font!" << "\n";
			exit(-1);
		}

		m_text.setCharacterSize(size);
		m_text.setFillColor(sf::Color(r, g, b));
	}

	void add_shape(const sf::Vector2f& pos, sf::Vector2f& vel, sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, float radius, std::string& text)
	{
		std::shared_ptr<sf::Shape> circle = std::make_shared<sf::CircleShape>(radius);
		circle->setPosition(pos);
		circle->setFillColor(sf::Color(r, g, b));
		Shape sCircle(circle, text, vel, m_viewSize);

		m_shapes.push_back(sCircle);
	}

	void add_shape(const sf::Vector2f& pos, sf::Vector2f& vel, sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Vector2f size, std::string& text)
	{
		std::shared_ptr<sf::Shape> rectangle = std::make_shared<sf::RectangleShape>(size);
		rectangle->setPosition(pos);
		rectangle->setFillColor(sf::Color(r, g, b, 255));

		Shape sRectangle(rectangle, text, vel, m_viewSize);

		m_shapes.push_back(sRectangle);
	}

	void info() {

	}
};


int main()
{
	Sence appSence;
	appSence.loadFromFile("sources.txt");
	appSence.Run();

	return 0;
}
