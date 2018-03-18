#pragma once

#include <SFML/Audio/Sound.hpp>

#include <string>
#include <memory>
#include <vector>

namespace sf
{
    class Sound;
    class Music;
}

class AssetManager;

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Class for playing game sounds.
GameAudio uses AssetManager to populate its list of sounds.

---------------------------------------------------------*/
class GameAudio final
{
using Sound = std::pair<std::string, std::unique_ptr<sf::Sound>>;

public:
    GameAudio(AssetManager& _asset_manager);
    ~GameAudio() = default;

    void playSound(const std::string& _file);

private:
    AssetManager& asset_manager;

    std::vector<Sound> sounds;


};
