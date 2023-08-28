#ifndef PLAYER_OWNER_H
#define PLAYER_OWNER_H

#include <game_project_1/component/component.hpp>

class PlayerOwner: public Component{
    public: 
    int player_id;

	PlayerOwner();
	PlayerOwner(int player);
	~PlayerOwner();

    Component* Clone();
    virtual void Clear();

    inline int SerializedLength(){return 0;}
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};




#endif