# Dagger

A fully-featured, modern game engine made for educational purposes.

## Dependencies

After cloning the project, create a `Deps` folder in the project folder and download the following dependencies inside it:

- [spdlog](https://github.com/gabime/spdlog)
- [entt](https://github.com/skypjack/entt/tree/master/src/entt)
- [sdl2](https://www.libsdl.org/)

The dependencies should be accessable through the following paths:

```bash
 $(ProjectPath)\Deps\spdlog\{include,src}
 $(ProjectPath)\Deps\entt\{src}
 $(ProjectPath)\Deps\sdl2\{include,lib} 
```

The Visual Studio solution will automatically look there and find the appropriate paths.

## Features

- Dagger is data-driven and event-based.
- Dagger is extremely modular.
- Dagger is clear and clearly educational.

## Rights and Reservations

Dagger is made as a part of Ubisoft Belgrade's game development course. All rights reserved.
