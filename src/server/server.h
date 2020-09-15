#ifndef SERVER_H
#define SERVER_H


#include "../game/scene.h"


class Server{

    public:

    int     max_players;
    int     scene_count;
    Scene*  active_scenes;


    Server();
    void Start();
    void LoadScene(int scene_id);
    void AddEntity(int eid);
    void RemoveEntity(int uuid);
    
    void Update(int ms);


};






#endif