#include "inSceneNode.h"
#include "inHumanView.h"

Shader* g_meshShader = nullptr;

SceneNode_Test::SceneNode_Test(unsigned int n_id) : SceneNode(n_id)
{
	if(!g_meshShader)
	{
		g_meshShader = g_shaderManager.AddShader("mesh","mesh_vertex.shader","","mesh_fragment.shader");
	}
}

void SceneNode_Test::VRender( double n_interpolation, glm::mat4x4& n_matrix )
{
	static float rotation = 0;
	static Mesh* tmp_mesh = g_meshManager.GetMesh("monkey");
	rotation += 0.001;

	glm::mat4x4 tmp_proj = glm::perspective((5.0f/360.0f) * 2 * PI, 16.0f/9.0f, 1.0f, 200.0f);
	glm::mat4x4 tmp_rot = glm::rotate(glm::mat4x4(1.0f),0.5f*PI,glm::vec3(1,0,0)) * glm::rotate(glm::mat4x4(1.0f),rotation*PI,glm::vec3(0,0,1));

	g_meshShader->Begin();
	g_meshShader->SetMat("modelview",n_matrix * tmp_rot);
	g_meshShader->SetMat("projection",tmp_proj);
	g_meshShader->SetMat("normalMat", glm::transpose(glm::inverse(n_matrix*tmp_rot)));

	tmp_mesh->Render();
	g_meshShader->End();
}