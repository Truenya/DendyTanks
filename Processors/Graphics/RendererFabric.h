
#include "AbstractRenderer.h"
#include "Renderer.h"

#ifndef RENDERER_FABRIC_H
#define RENDERER_FABRIC_H


enum RendererType {
    SDL,
    CONSOLE,
};

IRenderer* fabric_renderer(RendererType choice, std::atomic_bool &run);

/*template <typename E>
constexpr auto to_underlying_type_t<E>(e) noexept
{
    return static_cast <std::underlying_type_t<E>>(e);
}*/



//int conversion(Render a);

#endif //RENDERER_FABRIC_H