project(
  'Debuggable C Project',
  'c',
  version: '1.0',
  default_options: ['buildtype=debug', 'warning_level=2']
)

src_dir = 'src'

executable(
  'my_program',
  [src_dir + '/main.c',  + '/src.c'],
  include_directories: src_dir
)