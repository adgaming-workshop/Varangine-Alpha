#include <SFML/Graphics.hpp>
#include "physics.h"
#include "game_object.h"
#include "math.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <string>

/*segmentCollision (sf::Vector2f A1, sf::Vector2f A2, sf::Vector2f B1, sf::Vector2f B2){
        if (B1.x <= A1.x and A1.x <= B2.x or B2.x <= A1.x and A1.x <= B1.x or B1.x <= A2.x and A2.x <= B2.x or B2.x <= A2.x and A2.x <= B1.x){
                if(B1.y <= A1.y and A1.y <= B2.y or B2.y <= A1.y and A1.y <= B1.y or B1.y <= A2.y and A2.y <= B2.y or B2.y <= A2.y and A2.y <= B1.y){
                        return true;
                }
        }
        return false;
}*/

bool segmentCollision2(vector2f v11, vector2f v12, vector2f v21, vector2f v22, vector2f c1, vector2f c2)
{
  v11.x += c1.x;
  v11.y += c1.y;
  v12.x += c1.x;
  v12.y += c1.y;
  v21.x += c2.x;
  v21.y += c2.y;
  v22.x += c2.x;
  v22.y += c2.y;

  bool flag1 = ((((v12.x-v11.x)*(v21.y-v11.y)-(v21.x-v11.x)*(v12.y-v11.y))*((v12.x-v11.x)*(v22.y-v11.y)-(v12.y-v11.y)*(v22.x-v11.x))) <= 0);
  bool flag2 = ((((v21.x-v22.x)*(v11.y-v22.y)-(v21.y-v22.y)*(v11.x-v22.x))*((v21.x-v22.x)*(v12.y-v22.y)-(v21.y-v22.y)*(v12.x-v22.x))) <= 0);
  if (flag1 and flag2)
    return true;
  return false;
}

void PhysicsController::update(){
	auto beg = this->colliders.begin();
        auto end = this->colliders.end();
        for (auto it = beg; it != end; it++){
               	for (auto jt = it; jt != end; jt++){
			if(it != jt && (static_cast<Collider*>((*it)))->isCollided(static_cast<Collider*>(*jt))){
				static_cast<Collider*>(*it)->solveCollision(static_cast<Collider*>(*jt));
				static_cast<Collider*>(*it)->onCollision();
				//std::cout << "boom!" << std::endl;
			}
        	}
		static_cast<Collider*>(*it)->obj->prev_x -= static_cast<Collider*>(*it)->obj->x;
                static_cast<Collider*>(*it)->obj->prev_y -= static_cast<Collider*>(*it)->obj->y;


		static_cast<Collider*>(*it)->obj->velocity.x -= static_cast<Collider*>(*it)->obj->accel.x;//В этих 4 строчках были +=
		static_cast<Collider*>(*it)->obj->velocity.y -= static_cast<Collider*>(*it)->obj->accel.y;
		static_cast<Collider*>(*it)->obj->x -= static_cast<Collider*>(*it)->obj->velocity.x;
		static_cast<Collider*>(*it)->obj->y -= static_cast<Collider*>(*it)->obj->velocity.y;
	}
}

void solveInelasticCollision(vector2f norm, Collider* source, Collider* obj2){
	float module = sqrt(pow(norm.x, 2) + pow(norm.y, 2));
  	norm.x = norm.x/module;
  	norm.y = norm.y/module;
  	vector2f tau = {-norm.y, norm.x};

  	float v1n = source->obj->velocity.x * norm.x + source->obj->velocity.y * norm.y;
  	float v2n = obj2->obj->velocity.x * norm.x + obj2->obj->velocity.y * norm.y;
  	float v1t = source->obj->velocity.x * tau.x + source->obj->velocity.y * tau.y;
  	float v2t = obj2->obj->velocity.x * tau.x + obj2->obj->velocity.y * tau.y;
  	float vn = (source->mass * v1n + obj2->mass * v2n)/(source->mass + obj2->mass);
  	if ((obj2->moveable == false) and (source->moveable == true)){
    		source->obj->velocity.x = (v2n) *norm.x + v1t*tau.x;
    		source->obj->velocity.y = (v2n) *norm.y + v1t*tau.y;
  	}	

  	if ((obj2->moveable == true) and (source->moveable == false)){
		obj2->obj->velocity.x = (v1n) *norm.x + v2t*tau.x;
    		obj2->obj->velocity.y = (v1n) *norm.y + v2t*tau.y;
  	}	

  	if ((obj2->moveable == true) and (source->moveable == true))
  	{
   		source->obj->velocity.x = vn * norm.x + v1t*tau.x;
    		source->obj->velocity.y = vn * norm.y + v1t*tau.y;
    		obj2->obj->velocity.x = vn * norm.x + v2t*tau.x;
    		obj2->obj->velocity.y = vn * norm.y + v2t*tau.x;
  	}
}

void solveElasticCollision(vector2f norm, Collider* source, Collider* obj2){
	float module = sqrt(pow(norm.x, 2) + pow(norm.y, 2));
  	norm.x = norm.x/module;
  	norm.y = norm.y/module;
  	vector2f tau = {-norm.y, norm.x};
	
	//source->obj->x = source->obj->prev_x;
        //source->obj->y = source->obj->prev_y;
        //obj2->obj->x = obj2->obj->prev_x;
	//obj2->obj->y = obj2->obj->prev_y;

  	float v1n = source->obj->velocity.x * norm.x + source->obj->velocity.y * norm.y;
  	float v2n = obj2->obj->velocity.x * norm.x + obj2->obj->velocity.y * norm.y;
  	float v1t = source->obj->velocity.x * tau.x + source->obj->velocity.y * tau.y;
  	float v2t = obj2->obj->velocity.x * tau.x + obj2->obj->velocity.y * tau.y;

  	float k = source->mass / obj2->mass;
  	if (obj2->moveable == false)
    		k = 0;

  	float a = k+1;
  	float b = -2*v2n - 2*k*v1n;
  	float c = (k-1)*v1n*v1n + 2*v1n*v2n;

  	if ( (source->moveable == true)){
    		if (pow(b, 2) - 4*a*c < 0){
      			std::cout << "discriminant in elasticCollision < 0" << '\n';
      			return;
	       	}
    		float v1n_ = (-b + sqrt(pow(b, 2) - 4*a*c))/(2*a);
    		float v2n_ = k*(v1n - v1n_) + v2n;
    		source->obj->velocity.x = v1n_*norm.x + v1t*tau.x;
    		source->obj->velocity.y = v1n_*norm.y + v1t*tau.y;

    		obj2->obj->velocity.x = v2n_*norm.x + v2t*tau.x;
    		obj2->obj->velocity.y = v2n_*norm.y + v2t*tau.y;
  	}

  	if (source->moveable == false){
    		obj2->obj->velocity.x = 2*(v2n) *norm.x + v2t*tau.x;
    		obj2->obj->velocity.y = 2*(v2n) *norm.y + v2t*tau.y;
  	}
	std::cout << "boom! " << source->obj->name << " " << obj2->obj->name << std::endl;
}


void Collider::elasticCollision(Collider* source, Collider* obj2){
	if ((obj2->moveable == false) and (source->moveable == false))
    		return;
	//std::cout << "boom!" << std::endl;
  	vector2f V = {this->obj->velocity.x - obj2->obj->velocity.x, this->obj->velocity.y - obj2->obj->velocity.y};
	sf::ConvexShape vert1 = this->getModel();
	sf::ConvexShape vert2 = obj2->getModel();
  	vector2f c1 = {this->obj->x, this->obj->y};
  	vector2f c2 = {obj2->obj->x, obj2->obj->y};
  
  	for (int i = 0; i < vert1.getPointCount() - 1; i++){
    		for (int j = 0; j < vert2.getPointCount() - 1; j++){
			vector2f point_1i = {vert1.getPoint(i).x, vert1.getPoint(i).y};
			vector2f point_1iplus = {vert1.getPoint(i+1).x, vert1.getPoint(i+1).y};
			vector2f point_2j = {vert2.getPoint(j).x, vert2.getPoint(j).y};
			vector2f point_2jplus = {vert2.getPoint(j+1).x, vert2.getPoint(j+1).y};
      			if (segmentCollision2(point_1i, point_1iplus, point_2j, point_2jplus, c1, c2))
      			{		
        			vector2f norm = {-vert2.getPoint(j+1).y + vert2.getPoint(j).y, vert2.getPoint(j+1).x - vert2.getPoint(j).x};
        			if (((norm.x)*(-vert2.getPoint(j).x)+(norm.y)*(-vert2.getPoint(j).y)) > 0){	
          				norm.x = -norm.x;
          				norm.y = -norm.y;
        			}
        			if ((((norm.x)*(-vert2.getPoint(j).x)+(norm.y)*(-vert2.getPoint(j).y))*((norm.x)*(V.x)+(norm.y)*(V.y))) > 0) 
          				solveElasticCollision(norm, source, obj2); 
				
      			}
		}
	}	
 }

void Collider::inelasticCollision(Collider* source, Collider* obj2){

	if ((obj2->moveable == false) and (source->moveable == false))
    		return;
  	vector2f V = {this->obj->velocity.x - obj2->obj->velocity.x, this->obj->velocity.y - obj2->obj->velocity.y};

	sf::ConvexShape vert1 = this->getModel();
	sf::ConvexShape vert2 = obj2->getModel();
  	vector2f c1 = {this->obj->x, this->obj->y};
  	vector2f c2 = {obj2->obj->x, obj2->obj->y};

	for (int i = 0; i < vert1.getPointCount() - 1; i++){
    		for (int j = 0; j < vert2.getPointCount() - 1; j++){
      			vector2f point_1i = {vert1.getPoint(i).x, vert1.getPoint(i).y};
                        vector2f point_1iplus = {vert1.getPoint(i+1).x, vert1.getPoint(i+1).y};
                        vector2f point_2j = {vert2.getPoint(j).x, vert2.getPoint(j).y};
                        vector2f point_2jplus = {vert2.getPoint(j+1).x, vert2.getPoint(j+1).y};
                        if (segmentCollision2(point_1i, point_1iplus, point_2j, point_2jplus, c1, c2)){
        			vector2f norm = {-vert2.getPoint(j+1).y + vert2.getPoint(j).y, vert2.getPoint(j+1).x - vert2.getPoint(j).x};
        			if (((norm.x)*(-vert2.getPoint(j).x)+(norm.y)*(-vert2.getPoint(j).y)) > 0){
          				norm.x = -norm.x;
         				norm.y = -norm.y;
        			}
        			if ((((norm.x)*(-vert2.getPoint(j).x)+(norm.x)*(-vert2.getPoint(j).y))*((norm.x)*(V.x)+(norm.y)*(V.y))) > 0)
          				solveInelasticCollision(norm, source, obj2);
			}
		}
	}
}

void Collider::solveCollision(Collider* obj2){
	if ((obj2->elastic == true) and (this->elastic == true))
        	this -> elasticCollision(this, obj2);
      	else
        	this -> inelasticCollision(this, obj2);
}


void PhysicsController::appendCollider(Component* col){
	this->colliders.push_back(col);
}

void PhysicsController::removeCollider(Component* col){
	auto beg = this->colliders.begin();
	auto end = this->colliders.end();
	auto it_rm_col = find(beg, end, col);
	if(it_rm_col != end)
		this->colliders.erase(it_rm_col);
}
bool checkCrossing(vector2f v11, vector2f v12, vector2f v21, vector2f v22, vector2f c1, vector2f c2)
{
  v11.x += c1.x;
  v11.y += c1.y;
  v12.x += c1.x;
  v12.y += c1.y;
  v21.x += c2.x;
  v21.y += c2.y;
  v22.x += c2.x;
  v22.y += c2.y;

  bool flag1 = ((((v12.x-v11.x)*(v21.y-v11.y)-(v21.x-v11.x)*(v12.y-v11.y))*((v12.x-v11.x)*(v22.y-v11.y)-(v12.y-v11.y)*(v22.x-v11.x))) <= 0);
  bool flag2 = ((((v21.x -v22.x)*(v11.y -v22.y)-(v21.y-v22.y)*(v11.x-v22.x))*((v21.x-v22.x)*(v12.y-v22.y)-(v21.y-v22.y)*(v12.x-v22.x))) <= 0);
  if (flag1 and flag2)
    return true;
  return false;
}


bool Collider::isCollided(Collider* obj2){
	vector2f c1 = {this->obj->x, this->obj->y};
        vector2f c2 = {obj2->obj->x, obj2->obj->y};

	if (obj2->phys_model.getPointCount() == 0 or this->phys_model.getPointCount() == 0){
		return false;
	}

	 sf::ConvexShape v1 = this->phys_model;
        int n1 = v1.getPointCount();
        v1.setPoint(this->phys_model.getPointCount(), obj2->phys_model.getPoint(0));

        sf::ConvexShape v2 = obj2->phys_model;
        int n2 = v2.getPointCount();
        v2.setPoint(obj2->phys_model.getPointCount(), obj2->phys_model.getPoint(0));


    for (int i = 0; i < n1; i++)
        for (int j = 0; j < n2; j++)
        {
                vector2f first_i = {v1.getPoint(i).x, v1.getPoint(i).y};
                vector2f first_iplus ={v1.getPoint(i+1).x, v1.getPoint(i+1).y};
                vector2f second_j = {v2.getPoint(j).x, v2.getPoint(j).y};
                vector2f second_jplus = {v2.getPoint(j+1).x, v2.getPoint(j+1).y};

                if (checkCrossing(first_i, first_iplus, second_j, second_jplus, c1, c2))
                        return true;
        }
        return false;


	/*vector2f c1 = {this->obj->x, this->obj->y};
        vector2f c2 = {sample->obj->x, sample->obj->y};


	for (int i = 0; i < this->phys_model.getPointCount() - 1; i++){
                for (int j = 0; j < sample->phys_model.getPointCount() - 1; j++){
			auto a = sample->phys_model.getPoint(0);
			
			vector2f point_1i = {this->phys_model.getPoint(i).x, this->phys_model.getPoint(i).y};
                        vector2f point_1iplus = {this->phys_model.getPoint(i+1).x, this->phys_model.getPoint(i+1).y};
			vector2f point_2j = {sample->phys_model.getPoint(j).x, sample->phys_model.getPoint(j).y};
			vector2f point_2jplus = {sample->phys_model.getPoint(j+1).x, sample->phys_model.getPoint(j+1).y};

                        if (segmentCollision2(point_1i, point_1iplus, point_2j, point_2jplus, c1, c2)){
				return true;
                        }
                }
        }
        return false;*/
}




void Collider::makeModel(sf::ConvexShape sample){
	this->phys_model = sample;
}

sf::ConvexShape Collider::getModel(){
	return this->phys_model;
}

void Collider::setName(std::string name){
	this->name = name;
}

void Collider::setMov(bool flag){
	this->moveable = flag;
}

void Collider::setVisible(bool flag){
	this->visible = flag;
}
