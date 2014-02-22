import os
env = Environment()
env['CC'] = 'clang'
env['CXX'] = 'clang++'
env['ENV']['TERM'] = os.environ['TERM']
env['CXXFLAGS'].append(['-g', '-std=c++11', '-stdlib=libc++'])
env['CXXFLAGS'].append(['-O3'])
env['LINKFLAGS'].append(['-Wl,-no_pie', '-flto'])

env.Program('sentence', ['sentence.cpp', 'edge.cpp', 'node.cpp', 'graph.cpp'], LIBS=['tcmalloc', 'profiler'])
env.Program('client', ['client.cpp'])
