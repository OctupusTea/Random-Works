#include "mkl.h"
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 8000

double A[ WIDTH * WIDTH ];
double B[ WIDTH * WIDTH ];
double C[ WIDTH * WIDTH ];

int main( void )
{
	for( int i = 0; i < WIDTH; ++i )
	{
		A[ i * WIDTH + i ] = i;
		B[ i * WIDTH + i ] = 1;
	}
	
	cblas_dgemm( CblasColMajor, CblasNoTrans, CblasNoTrans, WIDTH, WIDTH, WIDTH, 1, A, WIDTH, B, WIDTH, 0, C, WIDTH );
	
	for( int i = 0; i < 80000; i += 8000 )
	{
		for( int j = 0; j < 10; ++j )
		{
			printf( "%0.lf ", C[ i + j ] );
		}
		puts( "" );
	}
	
	return 0;
}
