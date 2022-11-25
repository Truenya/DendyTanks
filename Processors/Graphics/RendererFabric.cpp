#include "RendererFabric.h"
#include <atomic>
#include <type_traits>
#include <cassert>
#include "AbstractRenderer.h"
#include "Renderer.h"
#include "CRenderer.h"


IRenderer* fabric_renderer(RendererType choice, std::atomic_bool &run)
{
 
switch(choice){
    case SDL: return new Renderer(run);
    break;
    case CONSOLE: return new CRenderer(run);
    break;
    }
}


    