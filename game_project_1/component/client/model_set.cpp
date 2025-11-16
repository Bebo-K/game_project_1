#include <game_project_1/component/client/model_set.hpp>
#include <game_project_1/types/math_types.hpp>
#include <game_project_1/core/entity.hpp>

ModelSet::ModelSet(Transform* parent):Drawable(parent){}
ModelSet::~ModelSet(){Clear();}

Model* ModelSet::Add(ModelRef model_id){
    return new (Allocate()) Model(&offset,model_id);
}

void ModelSet::Draw(Camera* cam){
    for(Model* m: (*this)){
        m->Draw(cam);
    }
}

Component* ModelSet::Clone(ComponentParentContext context){
    ModelSet* copy = new ModelSet(context.transform);
    copy->layer = layer;
    copy->hidden = hidden;
    copy->shader = copy->shader;
    for(Model* m: (*this)){copy->Add(m->type);}
    return copy;
}
void ModelSet::Clear(){this->Pool::Clear();}

