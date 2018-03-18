#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "GameState.h"
#include "LevelData.h"
#include "GUIButton.h"

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

The state of the program that oversees level selection.

---------------------------------------------------------*/
class GameStateSelection final : public GameState
{
public:
    explicit GameStateSelection(GameData& _game_data);
    virtual ~GameStateSelection() = default;

    void onStateEnter() override;
    void onStateLeave() override;

    void tick() override;
    void draw(sf::RenderWindow& _window) override;

private:
    void initLevelOptions();
    void initObjects();
    void initText();
    void initButtons();

    void quitGame();
    void playGame();

    void handleSelection();
    void selectOption(const int _index);
    void selectNext();
    void selectPrev();
    void updateSelectionDisplay();

    std::vector<std::unique_ptr<LevelData>> level_options;
    int selected_index;

    sf::Text title_display;

    // Level selection stuff.
    sf::Sprite level_snapshot;
    sf::Text level_pages_display;
    sf::Text level_name_display;
    sf::Text level_description_display;

    sf::Text level_duration_title;
    sf::Text level_duration_display;

    sf::Text level_score_title;
    sf::Text level_score_display;

    // Interactive stuff.
    GUIButton btn_next;
    GUIButton btn_prev;
    GUIButton btn_play;
    GUIButton btn_quit;

};
