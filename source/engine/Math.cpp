#include "Math.h"
#include <sstream>

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

void print(glm::vec3& n_vec)
{
	std::cout << n_vec.x << ", " << n_vec.y << ", " << n_vec.z << std::endl;
}

float toFloat(std::string n_string)
{
	std::stringstream s(n_string);
	float tmp_float;
	s >> tmp_float;
	return tmp_float;
}

int toInt(std::string n_string)
{
	std::stringstream s(n_string);
	int tmp_int;
	s >> tmp_int;
	return tmp_int;
}
