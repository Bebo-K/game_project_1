#ifndef MANIFEST_H
#define MANIFEST_H

#include <game_project_1/io/stream.hpp>
#include <game_project_1/types/str.hpp>
#include <game_project_1/io/json.hpp>

//Utility for reading JSON content manifest files
// "Content manifest files" here are defined as:
// .json files containing a single array of objects of the same type
// each object must contain an integer field "id" that is unique
// Code-side these objects are mapped to a type that implements
// -Default constructor() + dtor()
// -void FromJson(JsonObject* obj);

template<typename T>
void LoadManifest(const char* uri,const char* array,Map<int,T*>* map){
    char* path = cstr::new_copy(uri);
    FileStream manifest_file(path);
    JSONParser parser(&manifest_file);
    JSONObject* json_manifest = parser.Parse();
    JSONArray* data_list = json_manifest->GetArray(array);
    map->Resize(data_list->count);
    for(int i=0;i<data_list->count;i++){
        JSONObject* element = data_list->At(i)->ObjectValue();
        T* readElement = new T();
        readElement->FromJson(element);
        map->Add(element->GetInt("id"),readElement);
    }
    free(path);
    delete json_manifest;
}


#endif