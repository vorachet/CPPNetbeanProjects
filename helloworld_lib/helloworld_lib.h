/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   helloworld_lib.h
 * Author: vorachet
 *
 * Created on March 7, 2018, 3:32 PM
 */

#ifndef HELLOWORLD_LIB_H
#define HELLOWORLD_LIB_H

#include <iostream>

class helloworld_lib {
public:
    helloworld_lib();
    helloworld_lib(const helloworld_lib& orig);
    virtual ~helloworld_lib();
    void sayHello();
private:

};

#endif /* HELLOWORLD_LIB_H */

