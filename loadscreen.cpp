#include "loadscreen.h"

#include <fstream>

#include "audio.h"
#include "graphics.h"

Loadscreen::Loadscreen(const std::string& filename, Graphics& graphics, Audio& audio) : filename{filename} {
    load(graphics, audio);
}

void Loadscreen::load(Graphics& graphics, Audio& audio) {
    std::ifstream input{filename};
    if (!input) {
        throw std::runtime_error("Cannot open level filename: " + filename);
    }

    auto error_message = [](std::string filename, int line_num, std::string message, std::string line) { return filename + ":" + std::to_string(line_num) + " " + message + ": " + line; };

    int line_num{1};
    for (std::string line; std::getline(input, line); ++line_num) {
        if (line.empty()) {
            continue;
        }
        std::stringstream ss{line};
        std::string command;
        ss >> command;

        if (command == "background") {
            std::string filename;
            int scale, distance;
            ss >> filename >> scale >> distance;
            if (!ss) {
                std::string msg = error_message(filename, line_num, "Unable to load background", line);
                throw std::runtime_error(msg);
            }
            Sprite background = graphics.load_image("assets/" + filename);
            background.scale = scale;
            backgrounds.push_back({background, distance});
        } else if (command == "load-sounds") {
            std::string filename;
            ss >> filename;
            if (!ss) {
                std::string msg = error_message(filename, line_num, "Unable to load sounds", line);
                throw std::runtime_error(msg);
            }
            audio.load_sounds("assets/" + filename);
        }
    }
}