#ifndef ENTITY_COLLISION_H
#define ENTITY_COLLISION_H

#include <game_project_1/game/entity.hpp>
#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/server/server_scene.hpp>

namespace EntityCollision{

	struct EntityClassPair{
		int class_a;
		int class_b;
		bool operator==(EntityClassPair& b);
		bool operator==(int b);
	};

	typedef void (*ClientHandlerCallback)(ClientEntity*,ShapeCollider*, ClientEntity*,ShapeCollider*,ClientScene*,vec3);
	typedef void (*ServerHandlerCallback)(ServerEntity*,ShapeCollider*, ServerEntity*,ShapeCollider*,ServerScene*,vec3);

	struct CollisionResult{
		bool collided;
		vec3 point;
		int collider_index_1;
		int collider_index_2;

		CollisionResult(bool collided);
		CollisionResult(vec3 intersect,int indx_1,int indx_2);
	};

	//void ClientInit();
	//void ServerInit();

	void ClientFrame(ClientEntity* e1,ClientScene* s,float delta);
	void ServerFrame(ServerEntity* e1,ServerScene* s,float delta);
	CollisionResult CheckCollision(BaseEntity* e1, BaseEntity* e2);

	void RegisterClientEntityClassCallbacks(EntityClassPair pair,ClientHandlerCallback client_callback);
	void RegisterServerEntityClassCallbacks(EntityClassPair pair,ServerHandlerCallback server_callback);

};



#endif