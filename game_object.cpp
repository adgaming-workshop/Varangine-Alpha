#include "game_object.h"

GameObject::~GameObject(){
	Singleton_R_C::getInstance()->render_controller.removeObject(this);
	for (auto i = this->components.begin(); i != this->components.end(); i++){
		Singleton_R_C::getInstance()->script_controller.removeScript(i->second);
		Singleton_R_C::getInstance()->physics_controller.removeCollider(i->second);
		delete i->second;
	}
}

void GameObject::remove(){
	this->is_deleted = true;
}

void GameObject::removeAllComponents(){
	auto beg = this->components.begin();
	auto end = this->components.end();
	for(auto i = beg; i != end; i++){
		delete i->second;
		this->components.erase(i);
	}
	std::cout << "from removeAllComponents" << std::endl;
}

vector2f::vector2f(float x, float y){
        this->x = x;
        this->y = y;
}

