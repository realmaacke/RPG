project(
  'game-server',
  'cpp',
  version: '0.1.0',
  default_options: [
    'cpp_std=c++20',
    'warning_level=2',
    'buildtype=debugoptimized',
  ],
)

# sdl3_dep = dependency('sdl3', fallback: ['sdl3', 'sdl3_dep'])
enet_dep = dependency('enet', fallback: ['enet', 'enet_dep'])

sources = run_command('find', 'src', '-name', '*.cpp', check: true).stdout().strip().split('\n')

executable(
  'server',
  sources,
  dependencies: [enet_dep]
)