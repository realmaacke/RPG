project(
  'game-client',
  'cpp',
  version: '0.1.0',
  default_options: [
    'cpp_std=c++20',
    'warning_level=3',
    'buildtype=debugoptimized',
  ],
)

sdl3_dep = dependency('sdl3', fallback: ['sdl3', 'sdl3_dep'])
sdl3_image_dep = dependency('sdl3_image', fallback: ['sdl3_image', 'sdl3_image_dep'])
enet_dep = dependency('enet', fallback: ['enet', 'enet_dep'])


# RMLUI
cmake = import('cmake')
opts = cmake.subproject_options()
opts.add_cmake_defines({
  'RMLUI_SAMPLES': false,
  'RMLUI_TESTS': false,
})

rmlui_proj = cmake.subproject('rmlui', options: opts)
rmlui_dep = rmlui_proj.dependency('rmlui_core')

rmlui_backend_dir = 'include/Rmlui'
rmlui_backend_src = files(
  rmlui_backend_dir / 'RmlUi_Platform_SDL.cpp',
  rmlui_backend_dir / 'RmlUi_Renderer_SDL.cpp',
)

sources     = run_command('find', 'src', '-name', '*.cpp', check: true).stdout().strip().split('\n')
asset_files = run_command('find', 'assets', '-type', 'f', check: true).stdout().strip().split('\n')

custom_target(
  'assets',
  input: files(asset_files),
  output: 'assets',
  command: ['cp', '-rT', meson.current_source_dir() / 'assets', '@OUTPUT@'],
  build_by_default: true,
)

executable(
  'game',
  sources,
  rmlui_backend_src,
  dependencies: [sdl3_dep, sdl3_image_dep, enet_dep, rmlui_dep],
  include_directories: include_directories(rmlui_backend_dir),
  cpp_args: ['-DRMLUI_SDL_VERSION_MAJOR=3'],
)