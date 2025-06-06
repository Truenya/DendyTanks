#pragma once

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

// Only include SDL headers when not in test mode
#ifndef RESOURCE_MANAGER_TEST
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif


class ResourceManager
{
public:
	static std::string getMapFilePath (const std::string &mapName = "default")
	{
		// First try the exact path
		if (fileExists (mapName)) {
			return mapName;
		}

		// Try with resources prefix
		std::string resourcePath = "../resources/" + mapName;
		if (fileExists (resourcePath)) {
			return resourcePath;
		}

		// Try default maps
		if (mapName == "default" || mapName == "labirinth.txt") {
			// Try labirinth20x20.txt
			resourcePath = "../resources/labirinth20x20.txt";
			if (fileExists (resourcePath)) {
				std::cout << "Using default map: " << resourcePath << std::endl;
				return resourcePath;
			}

			// Try labirinth50x50.txt
			resourcePath = "../resources/labirinth50x50.txt";
			if (fileExists (resourcePath)) {
				std::cout << "Using default map: " << resourcePath << std::endl;
				return resourcePath;
			}
		}

		// If we get here, we couldn't find a suitable map file
		throw std::runtime_error ("Could not find map file: " + mapName +
		                          "\nPlease make sure the map file exists in the resources directory.\n"
		                          "Available maps: labirinth20x20.txt, labirinth50x50.txt");
	}

	static bool fileExists (const std::string &path)
	{
		std::ifstream file (path);
		return file.good ();
	}

	static std::string getBaseName (const std::string &path)
	{
		size_t pos = path.find_last_of ("/\\");
		if (pos != std::string::npos) {
			return path.substr (pos + 1);
		}
		return path;
	}

#ifndef RESOURCE_MANAGER_TEST
	static SDL_Texture *loadTexture (SDL_Renderer *renderer, const std::string &textureName)
	{
		// First try the exact path
		if (fileExists (textureName)) {
			return loadTextureFromPath (renderer, textureName);
		}

		// Try with resources prefix
		std::string resourcePath = "../resources/" + textureName;
		if (fileExists (resourcePath)) {
			return loadTextureFromPath (renderer, resourcePath);
		}

		// Try to find a matching texture in the resources directory
		std::string baseName = getBaseName (textureName);

		// Use unordered_map for texture mappings
		static const std::unordered_map<std::string, std::string> textureMap = {
		        {"tank_b.png", "../resources/tanks_b_green_blue_red_512x605.png"},
		        {"tank_t.png", "../resources/tanks_t_green_blue_red_512x605.png"},
		        {"tank_l.png", "../resources/tanks_l_green_blue_red_512x605.png"},
		        {"tank_r.png", "../resources/tanks_r_green_blue_red_512x605.png"},
		        {"explosion.png", "../resources/explosion.png"},
		        {"cvetok.png", "../resources/cvetok.png"},
		        {"fill.png", "../resources/fill.png"}};

		// Look up the texture path in the map
		auto it = textureMap.find (baseName);
		if (it != textureMap.end ()) {
			resourcePath = it->second;
			if (fileExists (resourcePath)) {
				if (baseName.find ("tank_") == 0) {
					std::cout << "Using texture: " << resourcePath << " for " << textureName << std::endl;
				}
				return loadTextureFromPath (renderer, resourcePath);
			}
		}

		// If we get here, we couldn't find a suitable texture
		throw std::runtime_error ("Could not find texture: " + textureName +
		                          "\nPlease make sure the texture file exists in the resources directory.");
	}

	static SDL_Texture *loadTextureFromPath (SDL_Renderer *renderer, const std::string &path)
	{
		SDL_Surface *surface = IMG_Load (path.c_str ());
		if (surface == nullptr) {
			throw std::runtime_error ("Failed to load image: " + path + "\nError: " + IMG_GetError ());
		}

		SDL_Texture *texture = SDL_CreateTextureFromSurface (renderer, surface);
		SDL_FreeSurface (surface);

		if (texture == nullptr) {
			throw std::runtime_error ("Failed to create texture from surface: " + path + "\nError: " + SDL_GetError ());
		}

		return texture;
	}
#endif
};
