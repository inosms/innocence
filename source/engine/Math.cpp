#include "Math.h"

void print(glm::mat4x4& n_mat)
{
	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			std::cout << n_mat[i][j] << ",\t";
		}
		std::cout << std::endl;
	}
}
