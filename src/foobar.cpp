#include "io/file.h"
#include "io/gltf.h"
//#include <iostream>

//using namespace std;

int foomain(int argc, char** args){
    File levelfile = File("dat/test_level.glb");

    GLTFScene gltf = GLTFScene();
    gltf.Load(levelfile);


    return 0;
}