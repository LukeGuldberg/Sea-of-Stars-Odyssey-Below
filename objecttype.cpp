#include "objecttype.h"

ObjectType create_object_type(Graphics &graphics, std::string type_name)
{
    if (type_name == "star")
    {
        return create_star(graphics);
    }
    else if (type_name == "coin")
    {
        return create_coin(graphics);
    }
    else
    {
        throw std::runtime_error("Unknown animated object type: " + type_name);
    }
}

ObjectType create_star(Graphics &graphics)
{
    AnimatedSprite sprite = graphics.get_animated_sprite("star", .15);
    return ObjectType{sprite};
}
ObjectType create_coin(Graphics &graphics)
{
    AnimatedSprite sprite = graphics.get_animated_sprite("coin", .15);
    return ObjectType{sprite};
}