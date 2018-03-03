#include <SFML/Graphics/RenderWindow.hpp>

#include "StateSelection.h"
#include "GameData.h"
#include "Constants.h"
#include "AssetManager.h"
#include "InputHandler.h"
#include "StateHandler.h"
#include "GameAudio.h"
#include "JHelper.h"
#include "JTime.h"
#include "FileIO.h"


StateSelection::StateSelection(GameData& _game_data)
    : State(_game_data)
    , selected_index(0)
{
    initLevelOptions();
    initObjects();
}


void StateSelection::onStateEnter()
{
    selectOption(selected_index);
}


void StateSelection::onStateLeave()
{
}


void StateSelection::tick()
{
    if (gameData().input.getKeyDown(sf::Keyboard::Escape))
        gameData().exit = true;

    if (gameData().input.getKeyDown(sf::Keyboard::Return))
    {
        gameData().audio.playSound(TOWER_BOOST_SOUND);
        getHandler()->queueState(GameState::GAME);
    }

    handleSelection();
}


void StateSelection::draw(sf::RenderWindow& _window)
{
    _window.draw(title_display);

    // Level selection stuff.
    _window.draw(title_display);

    _window.draw(level_snapshot);
    _window.draw(level_pages_display);

    _window.draw(level_name_display);
    _window.draw(level_description_display);

    _window.draw(level_duration_title);
    _window.draw(level_duration_display);

    _window.draw(level_score_title);
    _window.draw(level_score_display);

    // Interactive stuff.
    // TODO: ..
}


void StateSelection::initLevelOptions()
{
    auto option_names = FileIO::enumerateLevelNames();

    for (auto& name : option_names)
    {
        level_options.push_back(FileIO::loadLevelData(name));
    }

    selectOption(0);
}


void StateSelection::initObjects()
{
    initText();

    // Level snapshot.
    level_snapshot.setPosition(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.35f);
    level_snapshot.setScale(0.5f, 0.5f);
    JHelper::centerSFOrigin(level_snapshot);
}


void StateSelection::initText()
{
    auto* font = gameData().assets.loadFont(DEFAULT_FONT);

    // Level Selection title display.
    title_display.setString("Level Selection");
    title_display.setFont(*font);
    title_display.setCharacterSize(36);
    title_display.setPosition(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.085f);
    title_display.setOutlineColor(sf::Color::Black);
    title_display.setOutlineThickness(2);
    JHelper::centerSFOrigin(title_display);

    // Level pages display.
    level_pages_display.setFont(*font);
    level_pages_display.setCharacterSize(18);
    level_pages_display.setPosition(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.6f);
    level_pages_display.setOutlineColor(sf::Color::Black);
    level_pages_display.setOutlineThickness(2);
    JHelper::centerSFOrigin(level_pages_display);

    // Level name display.
    level_name_display.setFont(*font);
    level_name_display.setCharacterSize(32);
    level_name_display.setFillColor(sf::Color(255, 225, 25, 255));
    level_name_display.setPosition(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.66f);
    level_name_display.setOutlineColor(sf::Color::Black);
    level_name_display.setOutlineThickness(2);
    JHelper::centerSFOrigin(level_name_display);

    level_description_display.setFont(*font);
    level_description_display.setCharacterSize(20);
    level_description_display.setPosition(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.70f);
    level_description_display.setOutlineColor(sf::Color::Black);
    level_description_display.setOutlineThickness(2);
    JHelper::centerSFOrigin(level_description_display);

    // Level longest time survived display.
    level_duration_title.setFont(*font);
    level_duration_title.setCharacterSize(28);
    level_duration_title.setString("Longest Survived:");
    level_duration_title.setPosition(WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.75f);
    level_duration_title.setOutlineColor(sf::Color::Black);
    level_duration_title.setOutlineThickness(2);

    level_duration_display.setFont(*font);
    level_duration_display.setCharacterSize(28);
    level_duration_display.setString("0");
    level_duration_display.setPosition(WINDOW_WIDTH * 0.59f, WINDOW_HEIGHT * 0.75f);
    level_duration_display.setOutlineColor(sf::Color::Black);
    level_duration_display.setOutlineThickness(2);

    // Level highest score display.
    level_score_title.setFont(*font);
    level_score_title.setCharacterSize(28);
    level_score_title.setString("Highest Score:");
    level_score_title.setPosition(WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.8f);
    level_score_title.setOutlineColor(sf::Color::Black);
    level_score_title.setOutlineThickness(2);

    level_score_display.setFont(*font);
    level_score_display.setCharacterSize(28);
    level_score_display.setString("0");
    level_score_display.setPosition(WINDOW_WIDTH * 0.59f, WINDOW_HEIGHT * 0.8f);
    level_score_display.setOutlineColor(sf::Color::Black);
    level_score_display.setOutlineThickness(2);
}


void StateSelection::handleSelection()
{
    if (level_options.size() == 0)
        return;

    if (gameData().input.getKeyDown(sf::Keyboard::D))
        selectNext();

    if (gameData().input.getKeyDown(sf::Keyboard::A))
        selectPrev();
}


void StateSelection::selectOption(const int _index)
{
    if (!JHelper::validIndex(_index, level_options.size()))
        return;

    gameData().selected_level_data = level_options[_index].get();
    updateSelectionDisplay();
}


void StateSelection::selectNext()
{
    ++selected_index;

    if (selected_index >= static_cast<int>(level_options.size()))
        selected_index = 0;

    gameData().audio.playSound(TOWER_DESTROY_SOUND);
    selectOption(selected_index);
}


void StateSelection::selectPrev()
{
    --selected_index;

    if (selected_index < 0)
        selected_index = level_options.size() - 1;

    gameData().audio.playSound(TOWER_DESTROY_SOUND);
    selectOption(selected_index);
}


void StateSelection::updateSelectionDisplay()
{
    std::string selected_index_str = std::to_string(selected_index + 1);
    std::string level_options_size_str = std::to_string(level_options.size());

    level_pages_display.setString("(" + selected_index_str + " / " + level_options_size_str + ")");
    JHelper::centerSFOrigin(level_pages_display);

    auto* option = level_options[selected_index].get();
    auto* tex = gameData().assets.loadTexture("Levels/" + option->name + ".png");

    level_snapshot.setTexture(*tex);

    level_name_display.setString(option->name);
    JHelper::centerSFOrigin(level_name_display);

    level_description_display.setString(option->description);
    JHelper::centerSFOrigin(level_description_display);

    level_score_display.setString(std::to_string(option->highest_score));
    level_duration_display.setString(JHelper::timeToStr(option->session_duration));
}
