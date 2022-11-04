#include "Application.h"

using namespace GEngine;

int main(int argc, char* args[])
{
	AppSettings settings = AppSettings("My Game", new SceneBundle(), 500, 500, false);

	Application TestApp = Application(settings);
	TestApp.Run();

	return 0;
}