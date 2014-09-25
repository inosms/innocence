#include "SceneNode.h"
#include <iostream>

SceneNode::SceneNode(unsigned int n_id) :
	m_id( n_id),
	m_matrix(1.f),
	m_interpolationMatrix(0.f)
	{}

void SceneNode::RecursiveRenderCall_NonAlpha(double n_interpolation, glm::mat4x4& n_matrix, std::vector<std::pair<SceneNode*,glm::mat4x4> >& n_alphaNodes)
{
	glm::mat4x4 tmp_matrixApplied = n_matrix * m_matrix;
	glm::mat4x4 tmp_interpolatedMatrixAdd = m_interpolationMatrix;
	tmp_interpolatedMatrixAdd *= n_interpolation;
	tmp_matrixApplied += tmp_interpolatedMatrixAdd;

	if( HasAlpha() == true )
		n_alphaNodes.push_back(std::pair<SceneNode*,glm::mat4x4>(this,n_matrix));
	else
		VRender(n_interpolation,tmp_matrixApplied);

	for( SceneNode* i_node : m_children )
		i_node->RecursiveRenderCall_NonAlpha(n_interpolation,tmp_matrixApplied,n_alphaNodes);
}

void SceneNode::RecursiveRenderCall_All(double n_interpolation, glm::mat4x4& n_matrix)
{
	glm::mat4x4 tmp_matrixApplied = n_matrix * m_matrix;
	glm::mat4x4 tmp_interpolatedMatrixAdd = m_interpolationMatrix;
	tmp_interpolatedMatrixAdd *= n_interpolation;
	tmp_matrixApplied += tmp_interpolatedMatrixAdd;

	VRender(n_interpolation, tmp_matrixApplied);

	for( SceneNode* i_node : m_children )
		i_node->RecursiveRenderCall_All(n_interpolation,tmp_matrixApplied);

}

void SceneNode::RecursiveRenderCall_TopLevel(double n_interpolation,Camera& n_camera)
{
	glm::mat4x4 tmp_cameraMatrix = n_camera.GetMatrix(n_interpolation);

	std::vector<std::pair<SceneNode*,glm::mat4x4> > m_alphas;
	RecursiveRenderCall_NonAlpha(n_interpolation,tmp_cameraMatrix,m_alphas);

	// TODO sort m_alphas

	for( auto i_node : m_alphas )
	{
		i_node.first->RecursiveRenderCall_All(n_interpolation,i_node.second);
	}
}

void SceneNode::SetNewMatrix( glm::mat4x4 n_mat )
{
	m_oldMat = m_matrix;
	m_matrix = n_mat;
	m_interpolationMatrix = m_matrix - m_oldMat;
}

void SceneNode::SetNewMatrixNoInterpolation( glm::mat4x4 n_mat)
{
	m_oldMat = m_matrix = n_mat;
	m_interpolationMatrix = glm::mat4x4(0.0f);
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
	int i = 0;
	for( auto i_child : m_children )
	{
		if( i_child->m_id == n_id )
		{
			m_children.erase(m_children.begin()+i);
			return;
		}
		else
			i_child->RecursiveDeleteCall(n_id);
		i++;
	}
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

SceneNode::~SceneNode()
{
	for( SceneNode* i_child : m_children )
	{
		delete i_child;
	}
}
