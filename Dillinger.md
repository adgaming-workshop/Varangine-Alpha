# **Varangine** User API Docunentation
## Code example
When creating your game, code will be like that:
```
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <type_traits>
#include "user_api.h"


//  make your own scripts here

class MoveByKeys:public Script{ void execute() override;
};

void MoveByKeys::execute(){
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		this->obj->x -=0.3;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		this->obj->x +=0.3;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		this->obj->y -=0.3;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		this->obj->y +=0.3;
}




int main(){
    // creating game objects and setting game window parameters
	ge::createObject("ball");
	ge::deleteObject("ball");
	ge::createObject("ball");
	ge::setWindowTitle("Works!"); 
	ge::setWindowSize(800, 600);  
	// adjusting game objects' parameters
	ge::addComponent<Renderer>("ball"); 
	ge::addComponent<MoveByKeys>("ball");
	ge::makeSprite(ge::getObject("ball"), "image_2.png");
	ge::runApplication(); // starts main game cycle
	return 0;
}
```
## Components
### ge::Renderer
Applies graphics to game object. 
### ge::Collider
Apllies physics to game object. Incudes checking collision between object with such component.
For adding various collision solvings, make your own class and otnasleduyte it from `ge::Collider` class with creating method `onCollision()` in it.
Example:
```
class WallCollision: public Collider{ 
void onCollision() override;
};
WallCollision::onCollision(){
...your logics...
}
```
### ge::Scripts
You can add your own settings to game object by using scripts.
For this purpose, you need to fuck that shit up bruh
## Functions
### void ge::runApplication(); 
Opening main window

### bool makeSprite(GameObject* obj, std::string file_name);
Making sprite from file named file_name; This file should be in the project folder

### void createObject(std::string object_name);
Creating an object of class GameObject named 'object_name'

### void deleteObject(std::string object_name);
Deleting an object of class GameObject named 'object_name'

### GameObject* getObject(std::string object_name);
Finding an object of class GameObject named 'object_name'

### template <**typename T> void addComponent(std::string object_name);**
Adding component 'T' to game object 'object_name'. See **Components**
	
### template <**typename T> void removeComponent(std::string object_name);**
Removing component 'T' from game object 'object_name'. See **Components**
	
### void setWindowTitle(std::string title);
Setting window title 

### void setWindowSize(float x, float y);
Setting window size using coordinates x and y



