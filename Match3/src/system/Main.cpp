#include "System.h"

int main(void)
{
	System* system = new System();
	system->Initialise();
	system->Run();
	system->Terminate();
	return 0;
}