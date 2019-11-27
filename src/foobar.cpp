#include "io/data_structs.h"
#include <iostream>

using namespace std;

void PrintBitArray(BitArray* ba){

    printf("Bit array: ");
    for(int i=0;i<ba->bits;i++){
        cout << (ba->get(i))?"1":"0";
        cout << ",";
    }
    cout << endl;

}

int foomain(int argc, char** args){

    BitArray bit_array(16);

    int char_count = (16/8) + (((16 % 8) > 0)?1:0);
    printf("allocating %d bytes...\n",char_count);
    printf("Setting bit #3, 6, and 9\n");
    bit_array.set(3);
    bit_array.set(6);
    bit_array.set(9);
    
    printf("Un setting bit #2 and 9\n");
    bit_array.unset(2);
    bit_array.unset(9);

    printf("Toggling bit 5 and 9, only 3 and 5 should be set\n");
    bit_array.toggle(5);
    bit_array.toggle(6);

    PrintBitArray(&bit_array);

    return 0;
}