/*
 * i2c.cpp
 *
 * Created: 2020-09-09 오후 1:32:01
 * Author : PKNU
 */ 

#include <avr/io.h>
#include "i2c0slave.h"


int main(int argc, char **argv)
{
	// setup
	i2c0slave.begin(0x20);

	for(;;)
	{
		// loop

	}

	return 0;
}

