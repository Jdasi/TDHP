#pragma once

#include <vector>
#include <string>
#include <memory>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Class which maintains a list of all font, texture and sound assets
used by the game.

Assets are created when first requested. Thereafter, a pointer to the asset
is returned to avoid unecessary re-loading.

---------------------------------------------------------*/
class AssetManager final
{
    using Font = std::pair<std::string, std::unique_ptr<sf::Font>>;
    using Texture = std::pair<std::string, std::unique_ptr<sf::Texture>>;
    using SoundBuffer = std::pair<std::string, std::unique_ptr<sf::SoundBuffer>>;

public:
    AssetManager();
    ~AssetManager() = default;

    sf::Font* loadFont(const std::string& _file);
    sf::Texture* loadTexture(const std::string& _file);
    sf::SoundBuffer* loadSoundBuffer(const std::string& _file);

private:
    void init();

    std::vector<Font> fonts;
    std::vector<Texture> textures;
    std::vector<SoundBuffer> buffers;

};
