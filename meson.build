project('calc', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

readline = dependency('readline', required : true)

executable('calc',
           'src/calc.c',
           dependencies : readline,
           install : true)
