project('almondbread', 'cpp',
  version: '0.1',
  default_options: ['warning_level=3', 'werror=true', 'cpp_std=c++17', 'optimization=3'])

# TODO download gtest and sdl2

almondbread_exe = executable('almondbread',
  './src/Mandelbrot.hpp',
  './src/main.cpp',
  dependencies: [dependency('sdl2'), dependency('threads')],
  )

almondbread_test_exe = executable('AlmondbreadTest',
  './test/AlmondbreadTest.cpp',
  dependencies: [dependency('gtest_main')],
  include_directories: './src/',
  install: false,
  )
