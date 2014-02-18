import os
env = Environment()
env['CC'] = 'clang'
env['CXX'] = 'clang++'
env['ENV']['TERM'] = os.environ['TERM']
env['CXXFLAGS'].append(['-g', '-std=c++11', '-stdlib=libc++'])

env.Program('main', ['main.cpp', 'edge.cpp', 'node.cpp', 'graph.cpp'])
