#include <SFML/Graphics/RenderWindow.hpp>

#include "GameStateSelection.h"
#include "GameStateHandler.h"
#include "GameData.h"
#include "Constants.h"
#include "AssetManager.h"
#include "InputHandler.h"
#include "GameAudio.h"
#include "JHelper.h"
#include "JTime.h"
#include "FileIO.h"


GameStateSelection::GameStateSelection(GameData& _game_data)
    : GameState(_game_data)
    , selected_index(0)
{
    initLevelOptions();
    initObjects();
}


void GameStateSelection::onStateEnter()
{
    selectOption(selected_index);
}


void GameStateSelection::onStateLeave()
{
}


void GameStateSelection::tick()
{
    if (gameData().input.getKeyDown(sf::Keyboard::Escape))
    {
        quitGame();
    }

    if (gameData().input.getKeyDown(sf::Keyboard::Return))
    {
        playGame();
    }

    btn_next.tick(gameData());
    btn_prev.tick(gameData());
    btn_play.tick(gameData());
    btn_quit.tick(gameData());

    handleSelection();
}


void GameStateSelection::draw(sf::RenderWindow& _window)
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
    btn_next.draw(_window);
    btn_prev.draw(_window);
    btn_play.draw(_window);
    btn_quit.draw(_window);
}


void GameStateSelection::initLevelOptions()
{
    auto option_names = FileIO::enumerateLevelNames();

    for (auto& name : option_names)
    {
        level_options.push_back(FileIO::loadLevelData(name));
    }

    selectOption(0);
}


void GameStateSelection::initObjects()
{
    // Level snapshot.
    level_snapshot.setPosition(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.33f);
    level_snapshot.setScale(0.5f, 0.5f);
    JHelper::centerSFOrigin(level_snapshot);

    initText();
    initButtons();
}


void GameStateSelection::initText()
{
    auto* font = gameData().assets.loadFont(DEFAULT_FONT);

    // Level Selection title display.
    title_display.setString("Level Selection");
    title_display.setFont(*font);
    title_display.setCharacterSize(44);
    title_display.setPosition(level_snapshot.getPosition() - sf::Vector2f(0, 210));
    title_display.setFillColor(sf::Color(255, 225, 25, 255));
    title_display.setOutlineColor(sf::Color::Black);
    title_display.setOutlineThickness(2);
    JHelper::centerSFOrigin(title_display);

    // Level pages display.
    level_pages_display.setFont(*font);
    level_pages_display.setCharacterSize(18);
    level_pages_display.setPosition(level_snapshot.getPosition() + sf::Vector2f(0, 190));
    level_pages_display.setOutlineColor(sf::Color::Black);
    level_pages_display.setOutlineThickness(2);
    JHelper::centerSFOrigin(level_pages_display);

    // Level name display.
    level_name_display.setFont(*font);
    level_name_display.setCharacterSize(36);
    level_name_display.setFillColor(sf::Color(255, 225, 25));
    level_name_display.setPosition(level_pages_display.getPosition() + sf::Vector2f(0, 40));
    level_name_display.setOutlineColor(sf::Color::Black);
    level_name_display.setOutlineThickness(2);
    JHelper::centerSFOrigin(level_name_display);

    level_description_display.setFont(*font);
    level_description_display.setCharacterSize(20);
    level_description_display.setPosition(level_name_display.getPosition() + sf::Vector2f(0, 35));
    level_description_display.setOutlineColor(sf::Color::Black);
    level_description_display.setOutlineThickness(2);
    JHelper::centerSFOrigin(level_description_display);

    // Level longest time survived display.
    level_duration_title.setFont(*font);
    level_duration_title.setCharacterSize(28);
    level_duration_title.setString("Longest Survived:");
    level_duration_title.setPosition(WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.72f);
    level_duration_title.setOutlineColor(sf::Color::Black);
    level_duration_title.setOutlineThickness(2);

    level_duration_display.setFont(*font);
    level_duration_display.setCharacterSize(28);
    level_duration_display.setString("0");
    level_duration_display.setPosition(WINDOW_WIDTH * 0.59f, WINDOW_HEIGHT * 0.72f);
    level_duration_display.setOutlineColor(sf::Color::Black);
    level_duration_display.setOutlineThickness(2);

    // Level highest score display.
    level_score_title.setFont(*font);
    level_score_title.setCharacterSize(28);
    level_score_title.setString("Highest Score:");
    level_score_title.setPosition(WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.77f);
    level_score_title.setOutlineColor(sf::Color::Black);
    level_score_title.setOutlineThickness(2);

    level_score_display.setFont(*font);
    level_score_display.setCharacterSize(28);
    level_score_display.setString("0");
    level_score_display.setPosition(WINDOW_WIDTH * 0.59f, WINDOW_HEIGHT * 0.77f);
    level_score_display.setOutlineColor(sf::Color::Black);
    level_score_display.setOutlineThickness(2);
}


void GameStateSelection::initButtons()
{
    btn_next.setPosition(level_snapshot.getPosition() + sf::Vector2f(285, 0));
    btn_next.setColors(sf::Color(255, 225, 25, 255), sf::Color(255, 225, 100, 255), sf::Color(255, 225, 25, 150));
    btn_next.setTexture(gameData().assets.loadTexture(BTN_ARROW_TEXTURE));
    btn_next.setSize(sf::Vector2f(75, 175));
    btn_next.setRotation(180);
    btn_next.addClickEvent([this]() { selectNext(); });

    btn_prev.setPosition(level_snapshot.getPosition() - sf::Vector2f(285, 0));
    btn_prev.setColors(sf::Color(255, 225, 25, 255), sf::Color(255, 225, 100, 255), sf::Color(255, 225, 25, 150));
    btn_prev.setTexture(gameData().assets.loadTexture(BTN_ARROW_TEXTURE));
    btn_prev.setSize(sf::Vector2f(75, 175));
    btn_prev.addClickEvent([this](){ selectPrev(); });

    btn_play.setPosition(WINDOW_WIDTH * 0.4f, WINDOW_HEIGHT * 0.9f);
    btn_play.setColors(sf::Color(255, 225, 25, 255), sf::Color(255, 225, 100, 255), sf::Color(255, 225, 25, 150));
    btn_play.setTexture(gameData().assets.loadTexture(BTN_PLAY_TEXTURE));
    btn_play.setSize(sf::Vector2f(175, 75));
    btn_play.addClickEvent([this]() { playGame(); });

    btn_quit.setPosition(WINDOW_WIDTH * 0.6f, WINDOW_HEIGHT * 0.9f);
    btn_quit.setColors(sf::Color(255, 225, 25, 255), sf::Color(255, 225, 100, 255), sf::Color(255, 225, 25, 150));
    btn_quit.setTexture(gameData().assets.loadTexture(BTN_QUIT_TEXTURE));
    btn_quit.setSize(sf::Vector2f(175, 75));
    btn_quit.addClickEvent([this]() { quitGame(); });
}


void GameStateSelection::quitGame()
{
    gameData().exit = true;
}


void GameStateSelection::playGame()
{
    gameData().audio.playSound(TOWER_BOOST_SOUND);
    getHandler()->queueState(GAMESTATE_GAME);
}


void GameStateSelection::handleSelection()
{
    if (level_options.size() == 0)
        return;

    if (gameData().input.getKeyDown(sf::Keyboard::Right))
        selectNext();

    if (gameData().input.getKeyDown(sf::Keyboard::Left))
        selectPrev();
}


void GameStateSelection::selectOption(const int _index)
{
    if (!JHelper::validIndex(_index, level_options.size()))
        return;

    gameData().selected_level_data = level_options[_index].get();
    updateSelectionDisplay();
}


void GameStateSelection::selectNext()
{
    ++selected_index;

    if (selected_index >= static_cast<int>(level_options.size()))
        selected_index = 0;

    gameData().audio.playSound(TOWER_DESTROY_SOUND);
    selectOption(selected_index);
}


void GameStateSelection::selectPrev()
{
    --selected_index;

    if (selected_index < 0)
        selected_index = level_options.size() - 1;

    gameData().audio.playSound(TOWER_DESTROY_SOUND);
    selectOption(selected_index);
}


void GameStateSelection::updateSelectionDisplay()
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
