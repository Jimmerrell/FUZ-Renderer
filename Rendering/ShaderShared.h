#ifndef _SHADERSHARED_H
#define _SHADERSHARED_H


#define TESSELATION true

#define MAX_FUZ_PER_OBJECT 15
#define INT_BUFFER_SIZE ((MAX_FUZ_PER_OBJECT * 4) % 16) / 4 

#define MAX_FUZ_COUNT 200

// Old version
//#define FUZ_COUNT 10


struct INT4
{
	int arr[4];
};



#endif