#include "Application.h"

using namespace GEngine;

int main(int argc, char* args[])
{
	AppSettings settings =
	{
		"Test Game",
		640,
		480,
		false,
		"MainMenu"
	};

	Application TestApp = Application(settings);
	TestApp.Run();

	return 0;
}