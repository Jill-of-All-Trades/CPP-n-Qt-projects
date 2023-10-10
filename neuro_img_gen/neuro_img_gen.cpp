// neuro_img_gen.cpp

#include "ImgGen.h"
using namespace std;

int main()
{
    // MY
    //ImgGen img_gen(13, 13, 9, 9);
    //img_gen.generate();
    //img_gen.convert_to_csv("neuro_test_data.csv", ","); // neuro_data.csc

    // MY TEST
    /*ImgGen img_gen(13, 13, 9, 9);
    img_gen.test_n();
    img_gen.convert_to_csv("neuro_lab4_N.csv", ",");*/

    // A
    //ImgGen img_gen(14, 14, 7, 7);
    //img_gen.test2();
    //img_gen.convert_to_csv("A_neuro_test_data.csv", ","); // A_neuro_data.csv

    // A TEST
    ImgGen img_gen(14, 14, 7, 7);
    img_gen.test_a();
    img_gen.convert_to_csv("neuro_lab_A.csv", ","); // A_neuro_data.csv

    // I
    //ImgGen img_gen(14, 14, 7, 7);
    //img_gen.test3();
    //img_gen.convert_to_csv("I_neuro_test_data.csv", ","); // I_neuro_data.csv

    //img_gen.test2();
    return 0;
}


