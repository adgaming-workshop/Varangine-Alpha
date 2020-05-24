#include "user_api.h"

void ge::runApplication(){
	Singleton_R_C::runApplication();
}

bool ge::makeSprite(std::string object_name, std::string file_name){
	GameObject* obj = ge::getObject(object_name);
	if (obj->getComponent<Renderer>() != nullptr){
		obj->getComponent<Renderer>()->makeSprite(file_name);
		return true;
	}
	else return false;
}

void ge::createObject(std::string object_name){
	Singleton_R_C::getInstance()->data_storage.createObject(object_name);
}

void ge::deleteObject(std::string object_name){
	GameObject* obj = Singleton_R_C::getInstance()->data_storage.getObject(object_name);
	if (obj)
		obj->remove();
}

GameObject* ge::getObject(std::string object_name){
	return Singleton_R_C::getInstance()->data_storage.getObject(object_name);
}

void ge::setWindowSize(float x, float y){
	Singleton_R_C::getInstance()->render_controller.setWindowSize(x, y);
}

void ge::setWindowTitle(std::string title){
	Singleton_R_C::getInstance()->render_controller.setTitle(title);
}

void ge::setLayer(std::string object_name, int n){
	GameObject* obj = ge::getObject(object_name);
	if (n > Singleton_R_C::getInstance()->render_controller.getLayersCount())
		Singleton_R_C::getInstance()->render_controller.setLayersCount(n);
	if (obj->getComponent<Renderer>()){
		obj->getComponent<Renderer>()->setLayer(n);
	}
}

void ge::makeAnimation(std::string object_name, float x, float y, float w, float h, int count, float speed){
	GameObject* obj = ge::getObject(object_name);
	if (obj)
		obj->getComponent<Renderer>()->makeAnimation(x, y, w, h, count, speed);
}

void ge::setVelocity(std::string object_name, float vel_x, float vel_y){
	GameObject* obj = ge::getObject(object_name);
	if (obj)
		obj->velocity.x  = vel_x;
		obj->velocity.y = vel_y;
}

void ge::setAcceleration(std::string object_name, float acc_x, float acc_y){
        GameObject* obj = ge::getObject(object_name);
        if (obj)
                obj->velocity.x  = acc_x;
                obj->velocity.y = acc_y;
}

void ge::setMass (std::string object_name, float weight){
	GameObject* obj = ge::getObject(object_name);
        if (obj and obj->getComponent<Collider>())
                obj->getComponent<Collider>()->mass = weight;
}

void ge::setCoordinates(std::string object_name, vector2f coordinates){
	GameObject* obj = ge::getObject(object_name);
	if (obj){
		obj->x = coordinates.x ;
		obj->y = coordinates.y;
	}
}


