#include "level.h"
#include "graphics.h"
#include "audio.h"
#include "objecttype.h"
#include <fstream>
#include <algorithm>
#include <iostream>

// Level::Level() {}

Level::Level(const std::string &filename, Graphics &graphics, Audio &audio)
    : filename{filename}
{
    load(graphics, audio);
}

void Level::load(Graphics &graphics, Audio &audio)
{
    std::ifstream input{filename};
    // error if can't open file
    if (!input)
    {
        throw std::runtime_error("Could not read " + filename);
    }
    // load level's theme
    std::string theme_filename;
    input >> theme_filename;
    load_theme(theme_filename, graphics, audio);
    // read the level's layout
    std::vector<std::string> lines;
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
    // bool rectangular = std::all_of(std::begin(lines), std::end(lines), [=](const std::string &line)
    //                                { return static_cast<int>(line.size()) == width; });
    // error handling!

    for (int row = 0; row < height; ++row)
    {
        for (int x = 0; x < width; ++x)
        {
            char symbol = lines.at(row).at(x);
            if (std::isspace(symbol))
            {
                continue;
            }
            else if (symbol == 'P')
            { // player's starting position
                player_start_location = Vec<double>{static_cast<double>(x), static_cast<double>(height - row - 1)};
                continue;
            }
            else if (symbol == '*')
            {
                Vec<double> pos = Vec<double>{static_cast<double>(x), static_cast<double>(height - row - 1)};
                animated_objects.push_back(std::pair(pos, create_object_type(graphics, "star")));
            }
            else if (symbol == '$')
            {
                Vec<double> pos = Vec<double>{static_cast<double>(x), static_cast<double>(height - row - 1)};
                animated_objects.push_back(std::pair(pos, create_object_type(graphics, "coin")));
                continue;
            }
            else if (symbol == '#')
            {
                load_level_position = Vec<double>{static_cast<double>(x), static_cast<double>(height - row - 1)};
                continue;
            }

            // determine the tile type
            auto it = tile_types.find(symbol);
            auto eit = enemy_types.find(symbol);

            if (it != tile_types.end())
            {
                Vec<int> position{x, height - row - 1};
                // found it
                const Tile &tile = it->second;
                tiles.push_back({position, tile});
            }
            else if (eit != enemy_types.end())
            {
                Vec<double> position{static_cast<double>(x), static_cast<double>(height - 1 - row)};
                const EnemyType &type = eit->second(graphics);
                std::cout << enemies.size() << '\n';
                enemies.push_back({position, type});
            }
            else
            {
                throw std::runtime_error("Incorrect tile type ");
                // error
            }
        }
    }
    // error handling for player_starting_location == -1,-1
}

void Level::load_theme(const std::string &theme_filename, Graphics &graphics, Audio &audio)
{
    std::ifstream input{"assets/" + theme_filename};

    if (!input)
    {
        throw std::runtime_error("Unable to open " + theme_filename);
    }

    // nicely formatted error message
    auto error_message = [](std::string filename, int line_num, std::string message, std::string line)
    {
        return filename + ":" + std::to_string(line_num) + " " + message + ": " + line;
    };

    int line_num{1};
    for (std::string line; std::getline(input, line); ++line_num)
    {
        if (line.empty())
        {
            continue;
        }
        std::stringstream ss{line};
        std::string command;
        ss >> command;
        if (command == "load-spritesheet")
        {
            std::string filename;
            ss >> filename;
            if (!ss)
            {
                std::string msg = error_message(theme_filename, line_num, "Unable to load spritesheet", line);
                throw std::runtime_error(msg);
            }
            graphics.load_spritesheet("assets/" + filename);
        }
        else if (command == "load-sounds")
        {
            std::string filename;
            ss >> filename;
            if (!ss)
            {
                std::string msg = error_message(theme_filename, line_num, "Unable to load sounds", line);
                throw std::runtime_error(msg);
            }
            audio.load_sounds("assets/" + filename);
        }
        else if (command == "background")
        {
            std::string filename;
            int scale, distance;
            ss >> filename >> scale >> distance;
            if (!ss)
            {
                std::string msg = error_message(theme_filename, line_num, "Unable to load background", line);
                throw std::runtime_error(msg);
            }
            Sprite background = graphics.load_image("assets/" + filename);
            background.scale = scale;
            backgrounds.push_back({background, distance});
        }
        else if (command == "animated_background")
        {
            std::string file1, file2, file3;
            int scale;
            ss >> file1 >> file2 >> file3 >> scale;
            if (!ss)
            {
                std::string msg = error_message(theme_filename, line_num, "Unable to load background", line);
                throw std::runtime_error(msg);
            }
            Sprite bg1 = graphics.load_image("assets/" + file1);
            Sprite bg2 = graphics.load_image("assets/" + file2);
            Sprite bg3 = graphics.load_image("assets/" + file3);
            bg1.scale = scale;
            bg2.scale = scale;
            bg3.scale = scale;
            std::vector<Sprite> animated_backgrounds = {bg1, bg2, bg3};
            animated_background = {animated_backgrounds, .20};
        }
        else if (command == "enemy")
        {
            char symbol;
            std::string type_name;
            double acceleration;
            ss >> symbol >> type_name >> acceleration;
            if (!ss)
            {
                std::string msg = error_message(theme_filename, line_num, "unable to load enemy", line);
                throw std::runtime_error(msg);
            }
            auto generate_enemy_type = [=](Graphics &graphics)
            { return create_enemy_type(graphics, type_name); };
            enemy_types[symbol] = generate_enemy_type;
        }
        else if (command == "tile")
        {
            char symbol;
            std::string sprite_name;
            bool blocking;
            ss >> symbol >> sprite_name >> std::boolalpha >> blocking;
            if (!ss)
            {
                std::string msg = error_message(theme_filename, line_num, "Unable to load tile", line);
                throw std::runtime_error(msg);
            }
            Sprite sprite = graphics.get_sprite(sprite_name);
            Tile tile{sprite, blocking};

            // read possible commands
            std::string command_name;
            if (ss >> command_name)
            {
                std::vector<std::string> arguments;
                for (std::string argument; ss >> argument;)
                {
                    arguments.push_back(argument);
                }
                tile.command = create_command(command_name, arguments);
            }

            if (sprite_name == "hole")
            {
                *load_level_tile = tile;
                // tile_types[symbol] = *load_level_tile;
            }
            else
            {
                tile_types[symbol] = tile;
            }
        }
        else if (command == "animated_object")
        {
            char symbol;
            std::string sprite_name;
            ss >> symbol >> sprite_name;
            if (!ss)
            {
                std::string msg = error_message(theme_filename, line_num, "unable to load enemy", line);
                throw std::runtime_error(msg);
            }
            else
            {
                std::string msg = error_message(theme_filename, line_num, "Unknown command", line);
                throw std::runtime_error(msg);
            }
        }
    }
}