#ifndef ENTITY_COLLISION_H
#define ENTITY_COLLISION_H

#include <game_project_1/core/entity.hpp>
#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/server/server_scene.hpp>

namespace EntityCollision{
	
	typedef void (*ClientHandler)(ClientEntity*,ShapeCollider*, ClientEntity*,ShapeCollider*,ClientScene*,vec3);
	typedef void (*ServerHandler)(ServerEntity*,ShapeCollider*, ServerEntity*,ShapeCollider*,ServerScene*,vec3);

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
	CollisionResult CheckCollision(Entity* e1, Entity* e2);
	
	void RegisterClientLevelCollisionHandler(CollisionType coll_type, ClientHandler client_callback);
	void RegisterServerLevelCollisionHandler(CollisionType coll_type, ServerHandler server_callback);
};



#endif