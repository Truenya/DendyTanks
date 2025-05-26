#ifndef DENDY_TANKS_RENDER_COMPONENT_H
#define DENDY_TANKS_RENDER_COMPONENT_H

#include "../Component.h"
#include <string>
#include <SDL2/SDL.h>

/**
 * @class RenderComponent
 * @brief Component that represents the visual appearance of an entity
 */
class RenderComponent : public Component {
public:
    /**
     * @brief Constructor
     * @param textureId ID of the texture to use
     * @param width Width of the entity in pixels
     * @param height Height of the entity in pixels
     */
    RenderComponent(const std::string& textureId = "", int width = 32, int height = 32)
        : textureId_(textureId), width_(width), height_(height), visible_(true) {}
    
    /**
     * @brief Get the texture ID
     * @return Texture ID
     */
    const std::string& getTextureId() const { return textureId_; }
    
    /**
     * @brief Set the texture ID
     * @param textureId New texture ID
     */
    void setTextureId(const std::string& textureId) { textureId_ = textureId; }
    
    /**
     * @brief Get the width
     * @return Width in pixels
     */
    int getWidth() const { return width_; }
    
    /**
     * @brief Get the height
     * @return Height in pixels
     */
    int getHeight() const { return height_; }
    
    /**
     * @brief Set the size
     * @param width New width in pixels
     * @param height New height in pixels
     */
    void setSize(int width, int height) {
        width_ = width;
        height_ = height;
    }
    
    /**
     * @brief Check if the entity is visible
     * @return True if visible, false otherwise
     */
    bool isVisible() const { return visible_; }
    
    /**
     * @brief Set the visibility
     * @param visible True to make visible, false to hide
     */
    void setVisible(bool visible) { visible_ = visible; }
    
    /**
     * @brief Get the source rectangle
     * @return Source rectangle for rendering
     */
    SDL_Rect getSourceRect() const {
        return srcRect_;
    }
    
    /**
     * @brief Set the source rectangle
     * @param x X coordinate in the texture
     * @param y Y coordinate in the texture
     * @param width Width in the texture
     * @param height Height in the texture
     */
    void setSourceRect(int x, int y, int width, int height) {
        srcRect_.x = x;
        srcRect_.y = y;
        srcRect_.w = width;
        srcRect_.h = height;
    }

private:
    std::string textureId_;
    int width_;
    int height_;
    bool visible_;
    SDL_Rect srcRect_ = {0, 0, 0, 0};
};

#endif // DENDY_TANKS_RENDER_COMPONENT_H