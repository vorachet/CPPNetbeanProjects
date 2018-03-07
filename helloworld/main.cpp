/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: vorachet
 *
 * Created on March 7, 2018, 3:26 PM
 */

#include <iostream>

#include "helloworld_lib.h"

using namespace std;

int main(int argc, char** argv) {
    cout << "Hello world!" << endl;
    
    helloworld_lib lib;
    lib.sayHello();
    return 0;
}
