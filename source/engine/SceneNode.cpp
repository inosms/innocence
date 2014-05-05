#include "SceneNode.h"

SceneNode::SceneNode(unsigned int n_id) :
	m_id( n_id)
	{}

void SceneNode::RecursiveRenderCall_NonAlpha(double n_interpolation, glm::mat4x4& n_matrix, std::vector<std::pair<SceneNode*,glm::mat4x4> >& n_alphaNodes)
{
	glm::mat4x4 tmp_matrixApplied = n_matrix * m_matrix;

	if( HasAlpha() )
		n_alphaNodes.push_back(std::pair<SceneNode*,glm::mat4x4>(this,n_matrix));
	else
		VRender(n_interpolation,tmp_matrixApplied);

	for( SceneNode* i_node : m_children )
		i_node->RecursiveRenderCall_NonAlpha(n_interpolation,tmp_matrixApplied,n_alphaNodes);
}

void SceneNode::RecursiveRenderCall_All(double n_interpolation, glm::mat4x4& n_matrix)
{
	glm::mat4x4 tmp_matrixApplied = n_matrix * m_matrix;

	VRender(n_interpolation, tmp_matrixApplied);

	for( SceneNode* i_node : m_children )
		i_node->RecursiveRenderCall_All(n_interpolation,tmp_matrixApplied);

}

void SceneNode::RecursiveRenderCall_TopLevel(double n_interpolation,Camera& n_camera)
{
	glm::mat4x4 tmp_cameraMatrix = n_camera.GetMatrix();

	std::vector<std::pair<SceneNode*,glm::mat4x4> > m_alphas;
	RecursiveRenderCall_NonAlpha(n_interpolation,tmp_cameraMatrix,m_alphas);

	// TODO sort m_alphas

	for( auto i_node : m_alphas )
	{
		i_node.first->RecursiveRenderCall_All(n_interpolation,i_node.second);
	}
}

bool SceneNode::HasAlpha()
{
	return m_hasAlpha;
}

void SceneNode::SetAlpha( bool n_alpha )
{
	m_hasAlpha = n_alpha;
}

void SceneNode::RecursiveDeleteCall(unsigned int n_id)
{
	// TODO
}

void SceneNode::RecursiveUpdateCall()
{
	VUpdate();

	for( auto i_child : m_children )
		i_child->RecursiveUpdateCall();
}

SceneNode* SceneNode::RecursiveSearch( unsigned int n_id)
{
	if( m_id == n_id )
		return this;
	else if( m_children.empty() == true )
		return nullptr;
	else
	{
		for( SceneNode* i_child : m_children )
		{
			SceneNode* tmp_find = i_child->RecursiveSearch(n_id);
			if( tmp_find != nullptr )
				return tmp_find;
		}
	}
	return nullptr;
}

void SceneNode::AddChild(SceneNode* n_node)
{
	m_children.push_back(n_node);
}