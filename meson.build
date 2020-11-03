project('dagger', 'c', 'cpp', default_options : ['cpp_std=c++17' ])

deps = [
    dependency('glfw3'),
    dependency('fmt')
]

if (host_machine.system() == 'linux')
    deps += [
        dependency('tbb'),
        meson.get_compiler('c').find_library('dl')
    ]
endif

includes = [
    include_directories('source/dagger'),
    include_directories('libs/sparse-map/include'),
    include_directories('libs/entt/single_include'),
    include_directories('libs/glm/include'),
    include_directories('libs/glad/include'),
    include_directories('libs/json/include'),
    include_directories('libs/stb/include'),
    include_directories('libs/spdlog/include'),
    include_directories('libs/imgui/include'),
    include_directories('libs/imgui/include/imgui'),
    include_directories('libs/imgui/include/imgui/backends'),
    include_directories('libs/spdlog/include')
]

sources = [
    'source/dagger/main.cpp',
    'source/dagger/core/game/scene_management.cpp',
    'source/dagger/core/game/transforms.cpp',
    'source/dagger/core/graphics/animation.cpp',
    'source/dagger/core/graphics/animations.cpp',
    'source/dagger/core/graphics/gui.cpp',
    'source/dagger/core/graphics/shader.cpp',
    'source/dagger/core/graphics/shaders.cpp',
    'source/dagger/core/graphics/sprite_render.cpp',
    'source/dagger/core/graphics/sprite.cpp',
    'source/dagger/core/graphics/textures.cpp',
    'source/dagger/core/graphics/window.cpp',
    'source/dagger/core/input/inputs.cpp',
    'source/dagger/gameplay/examples/character_controller.cpp',
    'source/dagger/gameplay/examples/jiggle.cpp',
    'source/dagger/gameplay/ping_pong/ping_pong_main.cpp',
    'source/dagger/gameplay/ping_pong/pingpong_ball.cpp',
    'source/dagger/gameplay/ping_pong/pingpong_playerinput.cpp',
    'source/dagger/gameplay/ping_pong/player_scores.cpp',
    'source/dagger/gameplay/simple_collisions.cpp',
    'source/dagger/tools/console.cpp',
    'source/dagger/tools/diagnostics.cpp',
    'source/dagger/tools/plotvar.cpp',
    'source/dagger/tools/toolmenu.cpp',

    'libs/glad/include/glad/glad.c',
    'libs/imgui/include/imgui/imgui.cpp',
    'libs/imgui/include/imgui/imgui_widgets.cpp',
    'libs/imgui/include/imgui/imgui_draw.cpp',
    'libs/imgui/include/imgui/backends/imgui_impl_glfw.cpp',
    'libs/imgui/include/imgui/backends/imgui_impl_opengl3.cpp'
]

executable('dagger', sources, dependencies : deps, include_directories : includes, cpp_args : '-DIMGUI_IMPL_OPENGL_LOADER_GLAD')
