#include <stdio.h>
#include <stdlib.h>

#define WIDTH 8000

int A[ WIDTH ][ WIDTH ];
int B[ WIDTH ][ WIDTH ];
int C[ WIDTH ][ WIDTH ];

int main( void )
{
	for( int i = 0; i < WIDTH; ++i )
	{
		A[ i ][ i ] = i;
		B[ i ][ i ] = 1;
	}

	for( int i = 0; i < WIDTH; ++i )
	{
		for( int j = 0; j < WIDTH; ++j )
		{
			for( int k = 0; k < WIDTH; ++k )
			{
				C[ i ][ k ] += A[ i ][ j ] * B[ j ][ k ];
			}
		}
	}

	for( int i = 0; i < 10; ++i )
	{
		for( int j = 0; j < 10; ++j )
		{
			printf( "%d ", C[ i ][ j ] );
		}
		puts( "" );
	}
	
	return 0;
}
