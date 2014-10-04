#ifndef _SCREENLAYER_SCENE_H_
#define _SCREENLAYER_SCENE_H_

#include "ScreenLayer.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Process.h"
#include "Effect.h"

class FBO;
class ScreenLayer_Scene : public ScreenLayer
{
protected:
	Camera m_camera;
	SceneNode m_rootNode;
	FBO* m_fbo;
	EffectManager m_effectManager;

public:

	ScreenLayer_Scene();
	~ScreenLayer_Scene();

	// adds a sceneNode to the top level
	// -> root node -> n_node
	void AddSceneNode(SceneNode* n_node);

	// finds the id and attaches the node
	// to this id
	void AddSceneNode( SceneNode* n_node, unsigned n_childToID);

	// removes a node (with all children!)
	void RemoveSceneNode( unsigned int n_id );

	// finds not with given id returns nullptr if
	// not found
	SceneNode* FindSceneNode( unsigned int n_id);
	Camera& GetCamera();

	virtual void VUpdate();
	virtual void VRender( double n_interpolation );
	virtual bool VOnEvent(Event& n_event);
};

#endif
