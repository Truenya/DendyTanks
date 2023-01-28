#include "Processors/Graphics/Renderer.h"
#include "Processors/Common/Game.h"
#include <csignal>
static Game l;

int main () {
	signal (SIGINT, l.sigHandler);
	signal (SIGTERM, l.sigHandler);
	l.start();
	return 0;
}