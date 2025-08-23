#include "raylib_engine.h"
#include "tilemap.h"
#include "player.h"

int main(void) {
    Engine engine("uMotor Platformer", 400, 400);

    umotor::Scene& scene = engine.switch_scene<umotor::Scene>();

    std::string layout =\
    "#######################"\
    "#                     #"\
    "#                     #"\
    "#               #######"\
    "#         ####        #"\
    "#                     #"\
    "#   ####              #"\
    "#         ###         #"\
    "#         ####        #"\
    "#                     #"\
    "#              ########"\
    "#     ####     #       "\
    "################       ";

    scene.add_entity<TileMap>(layout, 23, 13);
    scene.add_entity<Player>(3, 3);

    engine.run();
}