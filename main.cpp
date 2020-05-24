#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <type_traits>
#include "user_api.h"
#include <cmath>




class MoveByKeys:public Script{ void execute() override;
};

void MoveByKeys::execute(){
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		this->obj->x -=30;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		this->obj->x +=30;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		this->obj->y +=30;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
		this->obj->y -=30;
		ge::deleteObject(this->obj->name);
	}
}

class CustomCollider:public Collider{
	void onCollision() override{
//		std::cout << "Collision!" << std::endl;
	}
	~CustomCollider(){};
};


int main(){
	ge::setWindowTitle("Works!");
	ge::setWindowSize(800, 600);

	std::vector<std::pair<float, float>> vec;
	float x, y, vx, vy;
	for(int j = 0; j < 6; j++){
		ge::createObject("rock" + std::to_string(j));
		ge::addComponent<Renderer>("rock" + std::to_string(j));
		ge::makeSprite("rock" + std::to_string(j), "rock.png");
		ge::makeAnimation("rock" + std::to_string(j), 0, 0, 64, 64, 16, 0.05);
		ge::addComponent<CustomCollider>("rock" + std::to_string(j));

		for (int i = 0; i < 4; i++){
			x = 40*cos((i+1)*3.14/4);
			y = 40*sin((i+1)*3.14/4);
			vec.push_back({x, y,});
		}
	


		ge::Vertex v = ge::Vertex(vec);
		ge::makeModelOfCollider<CustomCollider>(v, "rock" + std::to_string(j));

		x = -300;
		y = 0;
		ge::setCoordinates("rock" + std::to_string(j),{x + j*70, y});

		vx = rand() % 5500;
	      	vx /= 10000;
		vy = rand() % 5500;
		vy /= 10000;	
		ge::setVelocity("rock" + std::to_string(j), vx, vy);
	}

	ge::createObject("wall_1");
	ge::createObject("wall_2");
	ge::createObject("wall_3");
	ge::createObject("wall_4");

	std::vector<std::pair<float, float>> vec1, vec2, vec3, vec4;
	
	vec1.push_back({-400, 300});
	vec1.push_back({400, 300});
	vec1.push_back({400, 310});
	vec1.push_back({-400, 310});

	vec2.push_back({-399, 299});
	vec2.push_back({-410, 299});
	vec2.push_back({-410, -299});
	vec2.push_back({-399, -299});

	vec3.push_back({-400, -280});
	vec3.push_back({-400, -290});
	vec3.push_back({400, -290});
	vec3.push_back({400, -280});

	vec4.push_back({400, -299});
	vec4.push_back({410, -299});
	vec4.push_back({410, 299});
	vec4.push_back({400, 299});

	ge::Vertex v1 = ge::Vertex(vec1);
	ge::Vertex v2 = ge::Vertex(vec2);
	ge::Vertex v3 = ge::Vertex(vec3);
	ge::Vertex v4 = ge::Vertex(vec4);

	ge::addComponent<CustomCollider>("wall_1");
	ge::addComponent<CustomCollider>("wall_2");
	ge::addComponent<CustomCollider>("wall_3");
	ge::addComponent<CustomCollider>("wall_4");

	ge::makeModelOfCollider<CustomCollider>(v1, "wall_1");
	ge::makeModelOfCollider<CustomCollider>(v2, "wall_2");
	ge::makeModelOfCollider<CustomCollider>(v3, "wall_3");
	ge::makeModelOfCollider<CustomCollider>(v4, "wall_4");

	ge::setMoveable<CustomCollider>("wall_1", false);
	ge::setMoveable<CustomCollider>("wall_2", false);
	ge::setMoveable<CustomCollider>("wall_3", false);
	ge::setMoveable<CustomCollider>("wall_4", false);
	

	ge::runApplication();
	return 0;
}
