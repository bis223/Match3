#include "Application.h"

int main(void)
{
	Application* application = new Application();
	application->Initialise();
	application->Run();
	application->Terminate();
	return 0;
}