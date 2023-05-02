#ifndef PHYSICS_STATE_H
#define PHYSICS_STATE_H


//State information for entity phys simulation
class PhysicsState: public Component{
	public:
	bool in_bounds;
	bool midair;
	
	PhysicsState();
	~PhysicsState();

    Component* Clone();
    virtual void Clear();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};

#endif