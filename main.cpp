#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <sstream>
#include <string>
#include <map>
#include "board.h"
#include <time.h>
#include <cstdlib>
using namespace std;

void read_config(string filename, int& rows, int& cols, int& num_mines) {
    string s_rows, s_cols, s_num_mines;
    ifstream read_file(filename);
    if(read_file.is_open()) {
        getline(read_file, s_rows);
        getline(read_file, s_cols);
        getline(read_file, s_num_mines);
        rows = stoi(s_rows);
        cols = stoi(s_cols);
        num_mines = stoi(s_num_mines);
    }

}
void setText(sf::Text &text, float x, float y){ sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

bool display_welcome_window(sf::RenderWindow& window, const int height, const int width, const int num_mines, string& player_name, sf::Font& font) {
    // Blue Background
    window.clear(sf::Color::Blue);
    // Text
    // Welcome
    sf::Text welcome_text("WELCOME TO MINESWEEPER", font);
    welcome_text.setStyle(sf::Text::Bold);
    welcome_text.setStyle(sf::Text::Underlined);
    welcome_text.setCharacterSize(24);
    welcome_text.setFillColor(sf::Color::White);
    sf::Text enter_name("Enter your name:", font);
    setText(welcome_text, height/2, width/2 - 150);
    // Enter Name
    enter_name.setStyle(sf::Text::Bold);
    enter_name.setCharacterSize(20);
    enter_name.setFillColor(sf::Color::White);
    setText(enter_name, height/2, width/2 - 75);
    // Keyboard Position
    sf::Text keyboard_position("|", font);
    keyboard_position.setStyle(sf::Text::Bold);
    keyboard_position.setCharacterSize(18);
    keyboard_position.setFillColor(sf::Color::Yellow);
    setText(keyboard_position, height/2, width/2 - 45);
    // Draw
    window.draw(keyboard_position);
    window.draw(welcome_text);
    window.draw(enter_name);
    // Display
    window.display();
    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed: {
                    window.close();
                    break;
                }
                case sf::Event::TextEntered: {
                    if(((event.text.unicode >= 65 && event.text.unicode <= 90) || (event.text.unicode >= 97 && event.text.unicode <= 122) || (event.text.unicode == 8)) && (player_name.length() < 10 || (player_name.length() == 10 && event.text.unicode == 8))) {
                        if((event.text.unicode >= 65 && event.text.unicode <= 90) || (event.text.unicode >= 97 && event.text.unicode <= 122)) {
                            if(player_name.empty()) {
                                player_name += toupper((char)event.text.unicode);
                            }
                            else
                                player_name += tolower((char)event.text.unicode);
                        }
                        else if(event.text.unicode == 8 && !player_name.empty()) {
                            player_name.pop_back();
                        }
                        sf::Text name(player_name + "|", font);
                        name.setStyle(sf::Text::Bold);
                        name.setCharacterSize(18);
                        name.setFillColor(sf::Color::Yellow);
                        setText(name, height/2, width/2 - 45);
                        window.clear(sf::Color::Blue);
                        window.draw(name);
                        window.draw(welcome_text);
                        window.draw(enter_name);
                        window.display();
                    }
                }
                case sf::Event::KeyPressed: {
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !player_name.empty()) {
                        window.close();
                        return true;
                    }
                }
            }
        }
    }
}

map<string, sf::Texture> load_textures() {
    map<string, sf::Texture> textures;
    sf::Texture debug, digits, face_happy, face_lose, face_win, flag, leaderboard,
                mine, number_1, number_2, number_3, number_4, number_5, number_6, number_7,
                number_8, pause, play, tile_hidden, tile_revealed;
    debug.loadFromFile("files/images/debug.png"); textures.emplace("debug", debug);
    digits.loadFromFile("files/images/digits.png"); textures.emplace("digits", digits);
    face_happy.loadFromFile("files/images/face_happy.png"); textures.emplace("face_happy", face_happy);
    face_lose.loadFromFile("files/images/face_lose.png"); textures.emplace("face_lose", face_lose);
    face_win.loadFromFile("files/images/face_win.png"); textures.emplace("face_win", face_win);
    flag.loadFromFile("files/images/flag.png"); textures.emplace("flag", flag);
    leaderboard.loadFromFile("files/images/leaderboard.png"); textures.emplace("leaderboard", leaderboard);
    mine.loadFromFile("files/images/mine.png"); textures.emplace("mine", mine);
    number_1.loadFromFile("files/images/number_1.png"); textures.emplace("1", number_1);
    number_2.loadFromFile("files/images/number_2.png"); textures.emplace("2", number_2);
    number_3.loadFromFile("files/images/number_3.png"); textures.emplace("3", number_3);
    number_4.loadFromFile("files/images/number_4.png"); textures.emplace("4", number_4);
    number_5.loadFromFile("files/images/number_5.png"); textures.emplace("5", number_5);
    number_6.loadFromFile("files/images/number_6.png"); textures.emplace("6", number_6);
    number_7.loadFromFile("files/images/number_7.png"); textures.emplace("7", number_7);
    number_8.loadFromFile("files/images/number_8.png"); textures.emplace("8", number_8);
    pause.loadFromFile("files/images/pause.png"); textures.emplace("pause", pause);
    play.loadFromFile("files/images/play.png"); textures.emplace("play", play);
    tile_hidden.loadFromFile("files/images/tile_hidden.png"); textures.emplace("tile_hidden", tile_hidden);
    tile_revealed.loadFromFile("files/images/tile_revealed.png"); textures.emplace("tile_revealed", tile_revealed);
    return textures;
}

void assign_mines(board& game_board, int num_mines, const int rows, const int cols) {
    int count = 0;
    srand(time(0));
    while(count < num_mines) {
        int i = rand() % rows;
        int j = rand() % cols;
        if(!game_board.get_board()[i][j]->is_mine()) {
            game_board.get_board()[i][j]->set_mine(true);
            count++;
        }
    }
}

void set_adjacent_mines(board& game_board, const int rows, const int cols) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            int count = 0;
            if(!game_board.get_board()[i][j]->is_mine()) {
                // Check top left
                if(i > 0 && j > 0) {
                    if(game_board.get_board()[i-1][j-1]->is_mine()){
                        count++;
                    }
                    game_board.get_board()[i][j]->push_neighbor(game_board.get_board()[i-1][j-1]);
                }
                // Check Top
                if(j > 0) {
                    if(game_board.get_board()[i][j-1]->is_mine()){
                        count++;
                    }
                    game_board.get_board()[i][j]->push_neighbor(game_board.get_board()[i][j-1]);
                }
                // Check top right
                if(i < rows-1 && j > 0) {
                    if(game_board.get_board()[i+1][j-1]->is_mine()){
                        count++;
                    }
                    game_board.get_board()[i][j]->push_neighbor(game_board.get_board()[i+1][j-1]);
                }
                // Check Right
                if(i < rows-1){
                    if(game_board.get_board()[i+1][j]->is_mine()){
                        count++;
                    }
                    game_board.get_board()[i][j]->push_neighbor(game_board.get_board()[i+1][j]);
                }
                // Check Bottom right
                if(i < rows-1 && j < cols-1){
                    if(game_board.get_board()[i+1][j+1]->is_mine()){
                        count++;
                    }
                    game_board.get_board()[i][j]->push_neighbor(game_board.get_board()[i+1][j+1]);
                }
                // Check Bottom
                if(j < cols-1){
                    if(game_board.get_board()[i][j+1]->is_mine()){
                        count++;
                    }
                    game_board.get_board()[i][j]->push_neighbor(game_board.get_board()[i][j+1]);
                }
                // Check Bottom Left
                if(i > 0 && j < cols-1){
                    if(game_board.get_board()[i-1][j+1]->is_mine()){
                        count++;
                    }
                    game_board.get_board()[i][j]->push_neighbor(game_board.get_board()[i-1][j+1]);
                }
                // Check Left
                if(i > 0){
                    if(game_board.get_board()[i-1][j]->is_mine()){
                        count++;
                    }
                    game_board.get_board()[i][j]->push_neighbor(game_board.get_board()[i-1][j]);
                }
            }
            game_board.get_board()[i][j]->set_adjacent_mines(count);

        }
    }
}

vector<vector<string>> read_player_times() {
    ifstream read_file("files/leaderboard.txt");
    vector<vector<string>> player_times;
    string player_name;
    string time;
    while(read_file) {
        vector<string> temp_vec;
        getline(read_file, time, ',');
        getline(read_file, player_name, '\n');
        temp_vec.push_back(player_name);
        temp_vec.push_back(time);
        player_times.push_back(temp_vec);
    }
    return player_times;
}

void write_leaderboard(int time, string& player_name) {
    int minutes = time / 60;
    int secs = time % 60;
    string format_time;
    vector<vector<string>> player_times = read_player_times();
    if(minutes < 10) {
        if(secs > 9) {
            format_time = "0" + to_string(minutes) + ":" + to_string(secs);
        }
        else {
            format_time = "0" + to_string(minutes) + ":0" + to_string(secs);
        }
    }
    else {
        if(secs > 9) {
            format_time = to_string(minutes) + ":" + to_string(secs);
        }
        else {
            format_time = to_string(minutes) + ":0" + to_string(secs);
        }
    }
    string temp_time;
    for(int i = 0; i < player_times.size(); i++) {
        temp_time = player_times[i][1];
        string temp_minute, temp_second;
        stringstream s(temp_time);
        getline(s, temp_minute, ':');
        getline(s, temp_second);
        int temp_time_total_secs = stoi(temp_minute) * 60 + stoi(temp_second);
        if(temp_time_total_secs > time) {
            vector<string> temp_vec;
            temp_vec.push_back(player_name);
            temp_vec.push_back(format_time);
            player_times.insert(player_times.begin() + i, temp_vec);
            break;
        }
    }
    ofstream write_file("files/leaderboard.txt");
    for(int i = 0; i < 5; i++) {
        if(i == 4) {
            write_file << player_times[i][1] + "," + player_times[i][0];
        }
        else {
            write_file << player_times[i][1] + "," + player_times[i][0] << endl;
        }
    }
    write_file.close();
}

void display_pause(sf::RenderWindow& window, map<string, sf::Texture>& textures, board& game_board, const int rows, const int cols) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            game_board.get_board()[i][j]->tile_sprite.setTexture(textures.at("tile_revealed"));
            window.draw(game_board.get_board()[i][j]->tile_sprite);
        }
    }
}

bool display_leaderboard(const int rows, const int cols, sf::Font& font, bool win, int curr_time, string player_name) {
    int height = cols*16+50;
    int width = rows*16;
    sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
    window.clear(sf::Color::Blue);
    vector<vector<string>> player_times = read_player_times();
    if(win) {
        write_leaderboard(curr_time, player_name);
    }
    ifstream read_file("files/leaderboard.txt");
    string all_content;
    int count = 1;
    while(!read_file.eof()) {
        string player_name_temp;
        string time;
        getline(read_file, time, ',');
        getline(read_file, player_name_temp, '\n');
        all_content.append(to_string(count) + ".\t");
        all_content.append(time + "\t");
        if(player_name == player_name_temp) {
            player_name_temp += "*";
        }
        all_content.append(player_name_temp + "\n\n");
        count++;
    }
    sf::Text leaderboard_content(all_content, font);
    setText(leaderboard_content, (width/2)+60, (height/2)+90);
    leaderboard_content.setStyle(sf::Text::Bold);
    leaderboard_content.setCharacterSize(18);
    sf::Text leaderboard_header("LEADERBOARD", font);
    setText(leaderboard_header, (width/2)+35, (height/2)-110);
    leaderboard_header.setStyle(sf::Text::Bold);
    leaderboard_header.setStyle(sf::Text::Underlined);
    leaderboard_header.setCharacterSize(20);
    window.draw(leaderboard_header);
    window.draw(leaderboard_content);
    window.display();
    while(window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: {
                    window.close();
                    return true;
                    break;
                }
            }
        }
    }
}
int get_number_position(int num) {
    int position = 21 * num;
    return position;
}
void draw_counter(sf::RenderWindow& window, board& game_board, map<string, sf::Texture>& textures, int num_flags, int num_mines, const int cols) {
    string count = to_string(num_mines-num_flags);
    vector<char> split_count;
    if(stoi(count) > -1) {
        for(int i = 0; i < count.length(); i++) {
            split_count.push_back(count[i]);
        }
    } else {
        for(int i = 1; i < count.length(); i++) {
            split_count.push_back(count[i]);
        }
        sf::Sprite neg_sign;
        neg_sign.setPosition(21, (cols + 0.5) * 32 + 16);
        neg_sign.setTexture(textures.at("digits"));
        neg_sign.setTextureRect(sf::IntRect(10*21, 0, 21, 32));
        window.draw(neg_sign);
    }
    if(stoi(count) < 100 && stoi(count) > -1) {
        split_count.insert(split_count.begin(), '0');
        if(stoi(count) < 10 && stoi(count) > -1) {
            split_count.insert(split_count.begin(), '0');
        }
    }
    else if(stoi(count) < 0){
        if(stoi(count) > -10) {
            split_count.insert(split_count.begin(), '0');
            split_count.insert(split_count.begin(), '0');
        }
        else if(stoi(count) < -9) {
            split_count.insert(split_count.begin(), '0');
        }
    }
    cout << split_count[0] << split_count[1] << split_count[2] << endl;
    for(int i = 0; i < game_board.get_counter().size(); i++) {
        game_board.get_counter()[i]->digit_sprite.setTexture(textures.at("digits"));
        cout << split_count[i] << endl;
        int position = get_number_position(split_count[i] - '0');
        game_board.get_counter()[i]->digit_sprite.setTextureRect(sf::IntRect(position,0,21,32));
        window.draw(game_board.get_counter()[i]->digit_sprite);
    }
}

void draw_buttons(sf::RenderWindow& window, map<string, sf::Texture>& textures, board& game_board, bool& win, bool& playing, bool& paused) {
    game_board.get_button_map().at("face")->button_sprite.setTexture(textures.at("face_happy"));
    if(!paused) {
        game_board.get_button_map().at("play_pause")->button_sprite.setTexture(textures.at("pause"));
    }
    else {
        game_board.get_button_map().at("play_pause")->button_sprite.setTexture(textures.at("play"));
    }
    if(win) {
        game_board.get_button_map().at("face")->button_sprite.setTexture(textures.at("face_win"));
    }
    else if(!win && !playing) {
        game_board.get_button_map().at("face")->button_sprite.setTexture(textures.at("face_lose"));
    }
    game_board.get_button_map().at("debug")->button_sprite.setTexture(textures.at("debug"));
    game_board.get_button_map().at("leaderboard")->button_sprite.setTexture(textures.at("leaderboard"));
    window.draw(game_board.get_button_map().at("face")->button_sprite);
    window.draw(game_board.get_button_map().at("play_pause")->button_sprite);
    window.draw(game_board.get_button_map().at("debug")->button_sprite);
    window.draw(game_board.get_button_map().at("leaderboard")->button_sprite);
}

void draw_current_board(sf::RenderWindow& window, map<string, sf::Texture>& textures, const int rows, const int cols, board& game_board, bool debug_mode, bool win) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(game_board.get_board()[i][j]->is_flag()) {
                game_board.get_board()[i][j]->tile_sprite.setTexture(textures.at("tile_hidden"));
                game_board.get_board()[i][j]->flag_sprite.setTexture(textures.at("flag"));
                window.draw(game_board.get_board()[i][j]->tile_sprite);
                window.draw(game_board.get_board()[i][j]->flag_sprite);
            }
            else if(game_board.get_board()[i][j]->is_revealed()) {
                game_board.get_board()[i][j]->tile_sprite.setTexture(textures.at("tile_revealed"));
                window.draw(game_board.get_board()[i][j]->tile_sprite);
                if(game_board.get_board()[i][j]->is_mine() && !debug_mode) {
                    game_board.get_board()[i][j]->mine_sprite.setTexture(textures.at("mine"));
                    window.draw(game_board.get_board()[i][j]->mine_sprite);
                }
                if(game_board.get_board()[i][j]->get_adjacent_mines() > 0) {
                    int adjacent_mines = game_board.get_board()[i][j]->get_adjacent_mines();
                    game_board.get_board()[i][j]->number_sprite.setTexture(textures.at(to_string(adjacent_mines)));
                    window.draw(game_board.get_board()[i][j]->number_sprite);
                }
            }
            else if(!game_board.get_board()[i][j]->is_revealed()) {
                game_board.get_board()[i][j]->tile_sprite.setTexture(textures.at("tile_hidden"));
                window.draw(game_board.get_board()[i][j]->tile_sprite);
            }
            if(game_board.get_board()[i][j]->is_mine()) {
                if(debug_mode && !win) {
                    game_board.get_board()[i][j]->mine_sprite.setTexture(textures.at("mine"));
                    window.draw(game_board.get_board()[i][j]->mine_sprite);
                }
                if((debug_mode && win) || (!debug_mode && win)) {
                    game_board.get_board()[i][j]->mine_sprite.setTexture(textures.at("flag"));
                    window.draw(game_board.get_board()[i][j]->mine_sprite);
                }
            }
        }
    }
}

void display_game(sf::RenderWindow& window, map<string, sf::Texture>& textures, const int height, const int width, const int rows, const int cols, const int num_mines, const string& player_name, sf::Font& font) {
    window.create(sf::VideoMode(height, width), "Minesweeper", sf::Style::Close);
    window.clear(sf::Color::White);
    bool debug_mode = false;
    bool win = false;
    bool playing = true;
    bool paused = false;
    int num_flags = 0;
    int count = 0;
    sf::Clock clock;
    board game_board(rows, cols);
    int tiles_need_win = (rows * cols) - num_mines;
    cout << tiles_need_win << endl;
    // Assign Mines
    assign_mines(game_board, num_mines, rows, cols);
    set_adjacent_mines(game_board, rows, cols);
    // Draw Board
    draw_current_board(window, textures, rows, cols, game_board, debug_mode, win);
    draw_counter(window, game_board, textures, num_flags, num_mines, cols);
    draw_buttons(window, textures, game_board, win, playing, paused);
    // Display Board
    window.display();
    while(window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: {
                    window.close();
                    break;
                }
                case sf::Event::MouseButtonPressed: {
                    sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
                    if(playing && !paused) {
                        for(int i = 0; i < rows; i++) {
                            for(int j = 0; j < cols; j++) {
                                if(game_board.get_board()[i][j]->tile_sprite.getGlobalBounds().contains(mouse_position.x, mouse_position.y)) {
                                    cout << i << ", " << j << endl;
                                    if(!game_board.get_board()[i][j]->is_revealed()) {
                                        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                                            if(!game_board.get_board()[i][j]->is_flag()) {
                                                if(game_board.get_board()[i][j]->get_adjacent_mines() == 0) {
                                                    for(int k = 0; k < game_board.get_board()[i][j]->get_neighbors().size(); k++) {
                                                        if(!game_board.get_board()[i][j]->get_neighbors()[k]->is_flag()){
                                                            if(!game_board.get_board()[i][j]->get_neighbors()[k]->is_revealed()) {
                                                                game_board.get_board()[i][j]->get_neighbors()[k]->set_revealed(true);
                                                                tiles_need_win--;
                                                            }
                                                        }
                                                    }
                                                }
                                                game_board.get_board()[i][j]->set_revealed(true);
                                                tiles_need_win--;
                                                cout << tiles_need_win << endl;
                                                if(game_board.get_board()[i][j]->is_mine()) {
                                                    win = false;
                                                    playing = false;
                                                    debug_mode = true;
                                                }
                                                else if(tiles_need_win == 0) {
                                                    win = true;
                                                    playing = false;

                                                }
                                            }
                                        }
                                        else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                                            if(game_board.get_board()[i][j]->is_flag()) {
                                                game_board.get_board()[i][j]->set_flag(false);
                                                num_flags--;
                                            }
                                            else {
                                                game_board.get_board()[i][j]->set_flag(true);
                                                num_flags++;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if(game_board.get_button_map().at("debug")->button_sprite.getGlobalBounds().contains(mouse_position.x, mouse_position.y)) {
                            debug_mode = !debug_mode;
                        }
                    }
                    if(game_board.get_button_map().at("face")->button_sprite.getGlobalBounds().contains(mouse_position.x, mouse_position.y)) {
                        display_game(window, textures, height, width, rows, cols, num_mines, player_name, font);
                    }
                    if(game_board.get_button_map().at("play_pause")->button_sprite.getGlobalBounds().contains(mouse_position.x, mouse_position.y)) {
                        if(playing) {
                            paused = !paused;
                        }
                    }
                    if(game_board.get_button_map().at("leaderboard")->button_sprite.getGlobalBounds().contains(mouse_position.x, mouse_position.y)) {
                        window.clear(sf::Color::White);
                        if(!playing) {
                            display_leaderboard(rows, cols, font, win, clock.getElapsedTime().asSeconds(), player_name);
                        }
                        else {
                            display_pause(window, textures, game_board, rows, cols);
                            draw_buttons(window, textures, game_board, win, playing, paused);
                            draw_counter(window, game_board, textures, num_flags, num_mines, cols);
                            window.display();
                            if(display_leaderboard(rows, cols, font, win, clock.getElapsedTime().asSeconds(), player_name)) {
                                paused = false;
                            }
                        }
                    }
                    window.clear(sf::Color::White);
                    if(paused) {
                        display_pause(window, textures, game_board, rows, cols);
                    }
                    else {
                        draw_current_board(window, textures, rows, cols, game_board, debug_mode, win);
                    }
                    draw_buttons(window, textures, game_board, win, playing, paused);
                    draw_counter(window, game_board, textures, num_flags, num_mines, cols);
                    window.display();
                    if(win && count==0){
                        display_leaderboard(rows, cols, font, win, clock.getElapsedTime().asSeconds(), player_name);
                        count++;
                    }
                }
            }
        }
    }
}

int main() {
    int rows, cols, num_mines;
    read_config("files/config.cfg", rows, cols, num_mines);
    // Set Font
    sf::Font font;
    font.loadFromFile("files/font.ttf");
    int height = (rows * 32);
    int width = (cols * 32) + 100;
    string player_name;
    // Load Textures
    map<string, sf::Texture> textures = load_textures();
    sf::RenderWindow window(sf::VideoMode(height, width), "Minesweeper", sf::Style::Close);
    if(display_welcome_window(window, height, width, num_mines, player_name, font)) {
        display_game(window, textures, height, width, rows, cols, num_mines, player_name, font);
    }
    return 0;
}