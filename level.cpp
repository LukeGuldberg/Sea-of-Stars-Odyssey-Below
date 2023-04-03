#include "level.h"
#include "graphics.h"
#include "audio.h"
#include <fstream>
#include <algorithm>

Level::Level(const std::string &filename, Graphics &graphics, Audio &audio)
    : filename{filename}
{
    load(graphics, audio);
}

void Level::load(Graphics &graphics, Audio &audio)
{
    std::ifstream input{filename};
    // error if can't open file

    // load level's theme
    std::string theme_filename;
    input >> theme_filename;
    load_theme(theme_filename, graphics, audio);
    // read the level's layout
    std::vector<std::strings> lines;
    for (std::string line; std::getline(input, line);)
    {
        if (line.empty())
        {
            continue;
        }
        lines.push_back(line);
    }

    // set level dimensions
    height = lines.size();
    width = lines.front().size();

    // ensure rectangular
    bool rectangular = std::all_of(std::begin(lines), std::end(lines), [=](const std::string &line)
                                   { return static_cast<int>(line.size() == width;) });
    // error handling!

    for (int row = 0; row < height; ++row) {
        for(int x = 0; x < width, ++x) {
            char symbol = lines.at(row).at(x);
            if(std::isspace(symbol)) {
                continue;
            }
            if(symbol == 'P') { //player's starting position
                player_start_location = Vec<double>{static_cast<double>(x), static_cast<double>(height - row - 1)};
                continue;
            }
            // determine the tile type
            auto it = tile_types.find(symbol);
            if(it != tile_types.end()) {
                 //found it
                const Tile &tile = it->second;
                tiles.push_back({position, tile});
            }else {
                //error
            }
        }
    }
    //error handling for player_starting_location == -1,-1
}

void Level::load_theme(const std::string &theme_filename, Graphics &graphics, Audio &audio)
{
    std::ifstream input{"assets/" + theme_filename};
    // error if can't open

    std::string command;
    for (int line = 0; input >> command; ++line)
    {
        if (command == "load-spritesheet")
        {
            std::string filename;
            input >> filename;
            // if can't read in, error
            graphics.load_spritesheet("assets/" + filename)
        }
        else if (command == "load-sounds")
        {
            std::string filename;
            input >> filename;
            // if can't read in, error
            audio.load_sounds("assets/" + filename);
        }
        else if (command == "background")
        {
            std::string filename;
            int distance;
            input >> filename >> distance;
            // error handling
            Sprite background = graphics.load_image("assets/" + filename);
            background.scale = 5; // fix me
            backgrounds.push_back({background, distance});
        }
        else if (command == "tile")
        {
            char symbol;
            std::string sprite_name;
            bool blocking;
            input >> symbol >> sprite_name >> std::boolalpha >> blocking;
            Sprite sprite = graphics.get_sprite(sprite_name);
            tile_types[symbol] = Tile{sprite, blocking};
        }
        else
        {
            // error unknown command on line 'line'
            // filename:line "error message"
        }
    }
}