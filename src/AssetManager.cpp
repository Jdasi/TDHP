#include "JHelper.h"
#include "Constants.h"
#include "AssetManager.h"


AssetManager::AssetManager()
{
    init();
}


sf::Font* AssetManager::loadFont(const std::string& _file)
{
    auto entry = JHelper::findInVectorPair(fonts, _file);
    if (entry != fonts.end())
    {
        return entry->second.get();
    }

    auto font = std::make_unique<sf::Font>();
    font->loadFromFile(FONTS_PATH + _file);
    auto* p_font = font.get();

    fonts.emplace_back(_file, std::move(font));
    JHelper::sortVectorPair(fonts);

    return p_font;
}


sf::Texture* AssetManager::loadTexture(const std::string& _file)
{
    auto entry = JHelper::findInVectorPair(textures, _file);
    if (entry != textures.end())
    {
        return entry->second.get();
    }

    auto texture = std::make_unique<sf::Texture>();
    texture->loadFromFile(TEXTURES_PATH + _file);
    auto* p_texture = texture.get();

    textures.emplace_back(_file, std::move(texture));
    JHelper::sortVectorPair(textures);

    return p_texture;
}


sf::SoundBuffer* AssetManager::loadSoundBuffer(const std::string& _file)
{
    auto entry = JHelper::findInVectorPair(buffers, _file);
    if (entry != buffers.end())
    {
        return entry->second.get();
    }

    auto sound_buffer = std::make_unique<sf::SoundBuffer>();
    sound_buffer->loadFromFile(AUDIO_PATH + _file);
    auto* p_sound_buffer = sound_buffer.get();

    buffers.emplace_back(_file, std::move(sound_buffer));
    JHelper::sortVectorPair(buffers);

    return p_sound_buffer;
}


void AssetManager::init()
{
    // Preload fonts.
    loadFont(DEFAULT_FONT);

    // Preload sounds.
    loadSoundBuffer(BULLET_HIT_SOUND);
    loadSoundBuffer(BULLET_SHOOT_SOUND);
    loadSoundBuffer(PLAYER_DAMAGED_SOUND);
    loadSoundBuffer(HEALTH_BOOST_SOUND);
    loadSoundBuffer(SPEED_BOOST_SOUND);
    loadSoundBuffer(GAME_OVER_SOUND);
    loadSoundBuffer(LASER_SHOOT_SOUND);
    loadSoundBuffer(SMOKE_BOMB_SOUND);
    loadSoundBuffer(SWARM_SOUND);
    loadSoundBuffer(TOWER_BOOST_SOUND);
    loadSoundBuffer(TOWER_PLACE_SOUND);
    loadSoundBuffer(TOWER_DESTROY_SOUND);
    loadSoundBuffer(SCORE_BUMP_SOUND);
    loadSoundBuffer(TOWER_ERROR_SOUND);
    loadSoundBuffer(BS_NORMAL_SOUND);
    loadSoundBuffer(BS_ANGRY_SOUND);
    loadSoundBuffer(BS_EXHAUSTED_SOUND);
}
