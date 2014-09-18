#ifndef _MATH_H_
#define _MATH_H_

#include <iostream>

// this is required to remove a lot of
// warning messages of glm....
#define GLM_FORCE_RADIANS

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/matrix_inverse.hpp"


#define PI 3.1415f

extern void print(glm::mat4x4& n_mat);

#endif
