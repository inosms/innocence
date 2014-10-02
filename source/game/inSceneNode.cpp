#include "inSceneNode.h"
#include "inHumanView.h"

Shader* g_meshShader = nullptr;

SceneNode_Test::SceneNode_Test(unsigned int n_id, float n_width, float n_height) :
	SceneNode(n_id),
	m_width(n_width),
	m_height(n_height)
{
	if(!g_meshShader)
	{
		g_meshShader = g_shaderManager.AddShader("mesh","mesh_vertex.shader","","mesh_fragment.shader");
	}
}

void SceneNode_Test::VRender( double n_interpolation, glm::mat4x4& n_matrix )
{
	static MeshNode* tmp_mesh = g_meshNodeManager.GetMeshNode("monkey");

	glm::mat4x4 tmp_proj = glm::perspective((60.0f/360.0f) * 2 * PI, 16.0f/9.0f, 1.0f, 100.0f);
	glm::mat4x4 tmp_scale = glm::scale(glm::mat4x4(1.f),glm::vec3(m_width,m_height,1.f));

	g_meshShader->Begin();
		g_meshShader->SetMat("projection",tmp_proj);
		tmp_mesh->Render(g_meshShader,n_matrix * tmp_scale);
	g_meshShader->End();
}
