#define _USE_MATH_DEFINES

#include "aiming_system.h"

#include "core/graphics/sprite.h"
#include "core/input/inputs.h"

#include <cmath>
#include <math.h>

void AimingSystem::OnInitialize(Registry& registry_, Entity entity_)
{
    InputReceiver& receiver = registry_.get<InputReceiver>(entity_);
    for (auto command : { "rotate" })      //Input context has to have a key bound for "rotate" command (or you can rename the command to something you like)
    {
        receiver.values[command] = 0;
    }
}

void AimingSystem::Run()
{
    Engine::Registry().view<InputReceiver, Sprite, Crosshair>().each(
        [](const InputReceiver input_, Sprite& sprite_, Crosshair& crosshair_) //The sprite component with the same entity as this crosshair component is of a sprite that is used for center of rotation (like character for example)
        {
            Float32 rotate = input_.values.at("rotate");        //Get the input value of the rotate command - the amount the angle changes if the button is pressed 

            if (rotate) { 
                crosshair_.angle += rotate;   //Change the angle of the crosshair

                if (crosshair_.angle >= 2 * M_PI) {                                 //keep the angle in the first revolution
                    crosshair_.angle -= 2 * M_PI;
                }
                else if (crosshair_.angle < 0) {
                    crosshair_.angle = 2 * M_PI - (-crosshair_.angle);
                }


                auto& sprite = Engine::Registry().get<Sprite>(crosshair_.crosshairSprite); //get the crosshair spite component

                Float32 x = crosshair_.playerDistance * cos(crosshair_.angle);
                Float32 y = crosshair_.playerDistance * sin(crosshair_.angle);      //calculate the new x and y positions of the crosshair sprite based on the current angle , and offset them by the distance to the center of rotation


                sprite.position.x = x + sprite_.position.x;
                sprite.position.y = y + sprite_.position.y;     //move the center of rotation to the wanted sprite (character sprite for example)

                if (crosshair_.angle > 0.5 * M_PI && crosshair_.angle < 1.5 * M_PI) {     //change the scale of the sprite based on the current angle 
                    sprite_.scale = { -1,1 };
                }
                else {
                    sprite_.scale = { 1,1 };
                }


            }



        });
}

void AimingSystem::SpinUp()
{
    Engine::Registry().on_construct<InputReceiver>().connect<&AimingSystem::OnInitialize>(this);
}

void AimingSystem::WindDown()
{
    Engine::Registry().on_construct<InputReceiver>().disconnect<&AimingSystem::OnInitialize>(this);
}