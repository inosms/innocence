add_subdirectory(pugixml)

add_library( engine Application.cpp Camera.cpp Color.cpp Controller.cpp Creator.cpp Effect.cpp Error.cpp Event.cpp EventListener.cpp EventManager.cpp FBO.cpp Font.cpp GameLogic.cpp GameLoop.cpp GameObject.cpp GameView.cpp Math.cpp Menu.cpp Mesh.cpp Physics.cpp Process.cpp ProcessManager.cpp SceneNode.cpp ScreenLayer.cpp ScreenLayer_Scene.cpp Shader.cpp Texture.cpp Tickmeter.cpp UniqueNumberGenerator.cpp VideoSystem_SDL_OpenGL.cpp)
