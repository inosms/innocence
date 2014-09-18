#include "inSceneNode.h"
#include "inHumanView.h"

SceneNode_Test::SceneNode_Test(unsigned int n_id) : SceneNode(n_id)
{

}

void SceneNode_Test::VRender( double n_interpolation, glm::mat4x4& n_matrix )
{
	static float rotation = 0;
	static Mesh* tmp_mesh = g_meshManager.GetMesh("monkey");
	rotation += 0.003;

	glm::mat4x4 tmp_proj = glm::perspective((5.0f/360.f) * 2 * PI, 16.f/9.f, 1.f, 200.0f);
	glm::mat4x4 tmp_rot = glm::rotate(glm::mat4x4(1.0f),0.5f*PI,glm::vec3(1,0,0)) * glm::rotate(glm::mat4x4(1.0f),rotation*PI,glm::vec3(0,0,1));

/*
	test->SetMat("modelview",n_matrix * tmp_rot);
	test->SetMat("projection",tmp_proj);
	test->SetMat("normalMat", glm::transpose(glm::inverse(n_matrix*tmp_rot)));

	tmp_mesh->Render();
	*/
}