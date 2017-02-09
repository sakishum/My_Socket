#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "connecttionPool.h"

class TObject {
public:
	TObject(void) {
		++uid_;
	}
	~TObject(void) {}
	void release(void) {
		printf("release()\n");
	}
	long long getUID(void) const {
		return uid_;
	}

private:
	static long long uid_;
};

long long TObject::uid_ = 0;

int main(int, char**) {
	ConnectionPool<TObject> *objPool;
	objPool = new ConnectionPool<TObject>(2);
	TObject *pobj1 = objPool->alloc();
	if (pobj1) {
		std::cout << "1. " << pobj1->getUID() << std::endl;
	}
	TObject *pobj2 = objPool->alloc();
	if (pobj2) {
		std::cout << "2. " << pobj2->getUID() << std::endl;
	}
	TObject *pobj3 = objPool->alloc();
	if (pobj3) {
		std::cout << "3. " << pobj3->getUID() << std::endl;
	}
	objPool->recycle(pobj2);
	pobj3 = objPool->alloc();
	if (pobj3) {
		std::cout << "3.1. " << pobj3->getUID() << std::endl;
	}
	exit(EXIT_SUCCESS);
}
