#include "camera.h"
#include "graphics.h"
#include "tilemap.h"

Camera::Camera(Graphics& graphics, int tilesize)
    :graphics{graphics}, tilesize{tilesize} {
    calculate_visible_tiles();
    }
void Camera::move_to(const Vec<double>& new_location){
    location = new_location;
    calculate_visible_tiles();
}

Vec<int> Camera::world_to_screen(const Vec<double>& world_position) const{
    Vec<double> scaled =
        (world_position - location) * static_cast<double>(tilesize); //mapping my location to pixel location
    Vec<int> pixel{static_cast<int>(scaled.x), static_cast<int>(scaled.y)};

    //shift to the center of the screen
    pixel += Vec<int>{graphics.width / 2, graphics.height / 2};

    // flip the y axis
    pixel.y = graphics.height - pixel.y;
    return pixel;
}
