#include "spdlog/spdlog.h"


int main()
{
	//Use the default logger (stdout, multi-threaded, colored)
	spdlog::info("Hello, {}!", "World");
	spdlog::error("Some error message with arg: {}", 1);
}
