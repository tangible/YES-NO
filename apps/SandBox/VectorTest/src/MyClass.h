/*
 *  MyClass.h
 *  VectorTest
 *
 *  Created by Makira on 11/02/02.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "MySubClass.h"

class MyClass {

public:
	MyClass();
	MyClass(int i);
	~MyClass();
	string myClassID;
	vector<MySubClass*> mscptr;
	list<MySubClass*> mscptrlist;
};