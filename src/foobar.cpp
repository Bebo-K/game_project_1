#include "io/file.h"
#include "io/json.h"
#include <iostream>

using namespace std;


JsParser getJayson(){

    FILE *f = fopen("dat/test.json", "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *string = (char *)malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);

    string[fsize] = 0;
    return JsParser(string,fsize);
}



int main(int argc, char** args){

    JsParser parser = getJayson();

    JsObject* obj = parser.parse();
    obj->print(0);


    return 0;
}