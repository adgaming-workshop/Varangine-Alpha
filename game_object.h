#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <string>
#include <map>
#include "singleton.h"
#include <type_traits>

class vector2f{
public:
        float x;
        float y;
        vector2f(float x, float y);
};

class GameObject{
public:
	std::string name;
	template<typename T>
	void addComponent();
	template<typename T>
	void removeComponent();
	template<typename T>
	T* getComponent();
	float x = 0;
	float y = 0;
	float prev_x = 0;
	float prev_y = 0;
	vector2f velocity = vector2f(0, 0);
	vector2f accel = vector2f(0, 0);
	bool is_deleted = false;
	void remove();
	void removeAllComponents();
	~GameObject();
private:
	std::map<std::string, Component*> components;
};

template<typename T>
void GameObject:: addComponent(){
	if (this->components.find(typeid(T).name()) == this->components.end()){
		T* new_component = new T;
		new_component->obj = this;
		this->components[typeid(T).name()] = new_component;
		if (typeid(Renderer).name() == typeid(T).name())
			Singleton_R_C::getInstance()->render_controller.appendObject(this);
		if (std::is_base_of<Script, T>::value){
			Singleton_R_C::getInstance()->script_controller.appendScript(new_component);
		}
		if (std::is_base_of<Collider, T>::value){
			Singleton_R_C::getInstance()->physics_controller.appendCollider(new_component);
		}
	}
}

template<typename T>
void GameObject:: removeComponent(){
	if (this->components.find(typeid(T).name()) != this->components.end()){
		if (typeid(Renderer).name() == typeid(T).name())
			Singleton_R_C::getInstance()->render_controller.removeObject(this);
		if (std::is_base_of<Script, T>::value)
			Singleton_R_C::getInstance()->script_controller.removeScript(static_cast<T*>(this->components.find(typeid(T).name())->second));
		if (std::is_base_of<Collider, T>::value)
			Singleton_R_C::getInstance()->physics_controller.removeCollider(static_cast<T*>(this->components.find(typeid(T).name())->second));
		delete this->components[typeid(T).name()];
		this->components.erase(typeid(T).name());
	}
}
	
template<typename T>
T* GameObject::getComponent(){
	std::map<std::string, Component*>::iterator it;
	for (it = components.begin(); it != components.end(); it++)
		if (it->first == typeid(T).name())
			return static_cast<T*>(it->second);
	return nullptr;
}




#endif
