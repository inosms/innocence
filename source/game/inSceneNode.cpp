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
	if( !g_meshManager.GetMesh("sample_rect") )
	{
		// add the basic 1x1 white rect
		g_meshManager.AddMesh("sample_rect",Mesh::GetRect());
	}
}

void SceneNode_Test::VRender( double n_interpolation, glm::mat4x4& n_matrix )
{
	static float rotation = 0;
	static Mesh* tmp_mesh = g_meshManager.GetMesh("sample_rect");
//	rotation += 0.001;

	glm::mat4x4 tmp_proj = glm::perspective((10.0f/360.0f) * 2 * PI, 16.0f/9.0f, 1.0f, 200.0f);
	glm::mat4x4 tmp_rot = glm::rotate(glm::mat4x4(1.0f),rotation*PI,glm::vec3(0,0,1));
	glm::mat4x4 tmp_scale = glm::scale(glm::mat4x4(1.f),glm::vec3(m_width,m_height,1.f));

	g_meshShader->Begin();
	g_meshShader->SetMat("modelview",n_matrix * tmp_rot * tmp_scale);
	g_meshShader->SetMat("projection",tmp_proj);
	g_meshShader->SetMat("normalMat", glm::transpose(glm::inverse(n_matrix*tmp_rot*tmp_scale)));

	tmp_mesh->Render();
	g_meshShader->End();
}
