import os
env = Environment()
env['CC'] = 'clang'
env['CXX'] = 'clang++'
env['ENV']['TERM'] = os.environ['TERM']
env['CXXFLAGS'].append(['-g', '-std=c++11', '-stdlib=libc++'])
#env['CXXFLAGS'].append(['-O3'])
#env['LINKFLAGS'].append(['-Wl,-no_pie', '-flto'])

env.Library('markovdigraph', ['sentence.cpp', 'edge.cpp', 'node.cpp', 'graph.cpp'])
env.Library('client', ['client.cpp'])
env.Program('x', ['uncompress.cpp'], LINKFLAGS=['-lbz2'])
env.Program('slist', ['slist.cpp'])
env.Program('main', ['main.cpp'], LIBS=['markovdigraph', 'client'], LIBPATH=['.'])
