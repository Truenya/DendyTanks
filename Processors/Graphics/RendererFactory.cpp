#include "RendererFactory.h"

#ifdef MAKE_LOG
std::osyncstream& RendererFactory::logStream = *new std::osyncstream(std::cout);
#endif