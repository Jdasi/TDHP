#include "GameAudio.h"
#include "AssetManager.h"
#include "JHelper.h"


GameAudio::GameAudio(AssetManager& _asset_manager)
    : asset_manager(_asset_manager)
{
}


/* Plays a sound from Resources/Audio. Example usage: playSound("sound.wav")
 * A sound is constructed and then played if it is not already in the list.
 */
void GameAudio::playSound(const std::string& _file)
{
    auto entry = JHelper::findInVectorPair(sounds, _file);
    
    if (entry != sounds.end())
    {
        return entry->second->play();
    }

    auto sound = std::make_unique<sf::Sound>();
    sound->setBuffer(*asset_manager.loadSoundBuffer(_file));
    auto* p_sound = sound.get();

    sounds.emplace_back(_file, std::move(sound));
    p_sound->play();

    JHelper::sortVectorPair(sounds);
}
