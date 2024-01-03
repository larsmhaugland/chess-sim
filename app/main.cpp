#include "ChessApp.h"

/**
 * @brief Main function
 * @param argc - number of arguments
 * @param argv - arguments
 * @return
 */
int main(int argc, char* argv[])
{
	ChessApp application("Chess-sim", "1.0");

	application.Init();
	return application.Run();

}