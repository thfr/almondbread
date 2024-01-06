project('almondbread', 'cpp',
  version: '0.1',
  default_options: ['warning_level=3', 'werror=true', 'cpp_std=c++17', 'buildtype=debugoptimized'])

sdl2_sub = subproject('sdl2',
  default_options: ['cpp_std=c++17', 'werror=false']
  )
sdl2main_dep = sdl2_sub.get_variable('sdl2main_dep')
sdl2_dep = sdl2_sub.get_variable('sdl2_dep')

gtest_sub = subproject('gtest',
  default_options: ['cpp_std=c++17', 'werror=false']
  )
gtestmain_dep = gtest_sub.get_variable('gtest_main_dep')
gtest_dep = gtest_sub.get_variable('gtest_main_dep')

threads_dep = dependency('threads')

almondbread_exe = executable('almondbread',
  './src/Mandelbrot.hpp',
  './src/main.cpp',
  dependencies: [sdl2_dep, sdl2main_dep, threads_dep],
  )

almondbread_test_exe = executable('almondbreadtest',
  './test/AlmondbreadTest.cpp',
  dependencies: [gtestmain_dep, gtest_dep, threads_dep, sdl2_dep],
  include_directories: './src/',
  install: false,
  )
