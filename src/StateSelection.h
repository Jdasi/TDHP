#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "State.h"
#include "LevelData.h"

struct GameData;

class StateSelection final : public State
{
public:
    explicit StateSelection(GameData& _game_data);
    virtual ~StateSelection() = default;

    void onStateEnter() override;
    void onStateLeave() override;

    void tick() override;
    void draw(sf::RenderWindow& _window) override;

private:
    void initLevelOptions();
    void initObjects();
    void initText();

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
    // Left arrow ..
    // Right arrow ..
    // Play button ..
    // Quit button ..

};
