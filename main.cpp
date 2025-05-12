#include "helper/scene.h"
#include "helper/scenerunner.h"
#include "scenes/GameScene/gameScene.h"


int main(int argc, char* argv[])
{
	SceneRunner runner("OpenGL Game I Think Maybe");

	std::unique_ptr<Scene> scene;

	scene = std::unique_ptr<Scene>(new GameScene());
	return runner.run(*scene);
}