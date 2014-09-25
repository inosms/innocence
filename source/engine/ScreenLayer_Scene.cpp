#include "ScreenLayer_Scene.h"
#include "Error.h"

ScreenLayer_Scene::ScreenLayer_Scene() :
	ScreenLayer(ScreenLayer_Type_Scene),
	m_rootNode(0)
	{}

void ScreenLayer_Scene::AddSceneNode(SceneNode* n_node)
{
	m_rootNode.AddChild( n_node );
}

void ScreenLayer_Scene::AddSceneNode( SceneNode* n_node, unsigned n_childToID)
{
	SceneNode* tmp_find = FindSceneNode(n_childToID);
	if( tmp_find == nullptr )
	{
		UNIMPLEMENTED
	}
	else
		tmp_find->AddChild(n_node);

}

void ScreenLayer_Scene::RemoveSceneNode( unsigned int n_id )
{
	m_rootNode.RecursiveDeleteCall(n_id);
}

SceneNode* ScreenLayer_Scene::FindSceneNode( unsigned int n_id)
{
	return m_rootNode.RecursiveSearch(n_id);
}

void ScreenLayer_Scene::VUpdate()
{
	m_rootNode.RecursiveUpdateCall();
	m_camera.Update();
}

void ScreenLayer_Scene::VRender( double n_interpolation )
{
	m_rootNode.RecursiveRenderCall_TopLevel(n_interpolation,m_camera);
}

Camera& ScreenLayer_Scene::GetCamera()
{
	return m_camera;
}

bool ScreenLayer_Scene::VOnEvent(Event& n_event)
{
	// TODO:

	// not consumed...
	return false;
}
