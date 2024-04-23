//
// Created by Micah Tam on 4/12/24.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>
using namespace std;

class button {
public:
    sf::Sprite button_sprite;
};

class digit {
public:
    sf::Sprite digit_sprite;
};

class tile {
    bool mine;
    bool flag;
    bool revealed;
    int adjacent_mines;
    sf::Vector2f position;
    vector<tile*> neighbors;
public:
    sf::Sprite tile_sprite;
    sf::Sprite flag_sprite;
    sf::Sprite mine_sprite;
    sf::Sprite number_sprite;
    tile() {
        mine = false;
        flag = false;
        revealed = false;
        adjacent_mines = 0;
    }
    bool is_mine() {
        return mine;
    }
    bool is_flag() {
        return flag;
    }
    bool is_revealed() {
        return revealed;
    }
    int get_adjacent_mines() {
        return adjacent_mines;
    }
    sf::Vector2f get_position() {
        return position;
    }
    vector<tile*> get_neighbors() {
        return neighbors;
    }
    void set_mine(bool x) {
        if(x) {
            mine = true;
        }
        else
            mine = false;
    }
    void set_flag(bool x) {
        if(x) {
            flag = true;
        }
        else
            flag = false;
    }
    void set_revealed(bool x) {
        if(x) {
            revealed = true;
        }
        else
            revealed = false;
    }
    void set_position(int x, int y) {
        position.x = x;
        position.y = y;
    }
    void set_adjacent_mines(int x) {
        adjacent_mines = x;
    }
    void push_neighbor(tile* ptr) {
        neighbors.push_back(ptr);
    }
};

class board {
    vector<vector<tile*>> game_board;
    map<string, button*> button_map;
    vector<digit*> counter;
    vector<digit*> timer;
public:
    board(int rows, int cols) {
        // Tile Board
        for(int i = 0; i < rows; i++) {
            vector<tile*> temp_vec;
            for(int j = 0; j < cols; j++) {
                tile *temp_tile = new tile;
                temp_tile->set_position(i * 32, j * 32);
                temp_tile->tile_sprite.setPosition(temp_tile->get_position());
                temp_tile->flag_sprite.setPosition(temp_tile->get_position());
                temp_tile->mine_sprite.setPosition(temp_tile->get_position());
                temp_tile->number_sprite.setPosition(temp_tile->get_position());
                temp_vec.push_back(temp_tile);
            }
            game_board.push_back(temp_vec);
        }
        // Buttons
        button *face = new button;
        button *debug = new button;
        button *play_pause = new button;
        button *leaderboard = new button;
        face->button_sprite.setPosition(((rows/2.0) * 32) - 32, (cols + 0.5) * 32);
        debug->button_sprite.setPosition((rows*32)-304, (cols + 0.5) * 32);
        play_pause->button_sprite.setPosition((rows*32)-240, (cols + 0.5) * 32);
        leaderboard->button_sprite.setPosition((rows*32)-176, (cols + 0.5) * 32);
        button_map.emplace("face", face);
        button_map.emplace("debug", debug);
        button_map.emplace("play_pause", play_pause);
        button_map.emplace("leaderboard", leaderboard);
        // Counter
        for(int i = 0; i < 3; i++) {
            digit *temp_digit = new digit;
            temp_digit->digit_sprite.setPosition(21*i+42, (cols + 0.5) * 32 + 16);
            counter.push_back(temp_digit);
        }
        // Timer
        for(int i = 0; i < 4; i++) {
            digit *temp_digit = new digit;
            if(i <= 1) {
                temp_digit->digit_sprite.setPosition(21*i+rows*32-97, (cols + 0.5) * 32 + 16);
            }
            else {
                temp_digit->digit_sprite.setPosition(21*i+rows*32-95, (cols + 0.5) * 32 + 16);
            }
            timer.push_back(temp_digit);
        }
    }
    vector<vector<tile*>> get_board() {
        return game_board;
    }
    map<string, button*> get_button_map() {
        return button_map;
    }
    vector<digit*> get_counter() {
        return counter;
    }
    vector<digit*> get_timer() {
        return timer;
    }
};
