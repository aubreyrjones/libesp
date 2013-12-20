import os.path
import platform

esp_env = Environment(tools=['default','textfile'], CPPPATH=['#/src'])
esp_env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME']=1
uplift_env = None #different build parameters for programs/libs on different systems

def filtered_glob(env, pattern, omit=[], ondisk=True, source=False, strings=False):
    return filter(lambda f: os.path.basename(f.path) not in omit, env.Glob(pattern))

def prefix_with(prefix):
    return lambda x: prefix + x

esp_env.AddMethod(filtered_glob, "FilteredGlob")

build_platform = None
build_debug = (ARGUMENTS.get('debug', 'false') == 'true')
build_tests = (ARGUMENTS.get('tests', 'false') == 'true')
build_utilities = (ARGUMENTS.get('tests', 'true') == 'true')

if esp_env['PLATFORM'] == 'posix':
    esp_env.Replace(CXX = "g++-4.8")
    esp_env.Replace(CC = "gcc-4.8")
    build_platform = 'posix'
elif esp_env['PLATFORM'] == 'win32':
    build_platform = 'win'
    esp_env['ENV'] = os.environ
elif esp_env['PLATFORM'] == 'darwin':
    esp_env.Replace(CXX = "clang")
    build_platform = 'mac'

esp_libs = []

#==================LIBRARIES=============
if build_platform == 'posix':
    esp_libs = ['rt', 'pthread', 'dl']


#=================COMPILER FLAGS=========
def add_linux_common_build_options(environ):
    environ.AppendUnique(CPPDEFINES = ['ESP_LINUX'])
    environ.AppendUnique(CCFLAGS = Split('-fPIC'))
    environ.AppendUnique(CXXFLAGS = Split('-m64 -std=c++11 -pedantic -fno-rtti'))
    environ.AppendUnique(LINKFLAGS = Split('-rdynamic -fno-rtti'))
    if build_debug:
        environ.AppendUnique(CCFLAGS = Split('-g'))
    else:
        environ.AppendUnique(CCFLAGS = Split('-O3'))
    
def add_windows_common_build_options(environ):
    environ.AppendUnique(CPPDEFINES = ['ESP_WINDOWS', 'HAVE_READLINE=0'])
    environ.AppendUnique(CCFLAGS = Split('/GR- /EHsc /MD'))
    if build_debug:
        environ.AppendUnique(CCFLAGS = Split('/Z7'))
    else:
        environ.AppendUnique(CCFLAGS = Split('/O2'))


if build_platform == 'posix':    
    add_linux_common_build_options(esp_env)
    uplift_env = esp_env.Clone()
    uplift_env.AppendUnique(CPPDEFINES = ['HAVE_READLINE=1'])
    
    esp_env.AppendUnique(CXXFLAGS = Split('-fno-exceptions'))
    esp_env.AppendUnique(LINKFLAGS = Split('-fno-exceptions'))
    
elif build_platform == 'win':
    add_windows_common_build_options(esp_env)
    uplift_env = esp_env.Clone()
    
#==============ESP STATIC LIBRARY===================

esp_sources = esp_env.Glob("#/src/*.cpp")
esp_objects = esp_env.Object(esp_sources)

esp_lib = esp_env.Library("esp", esp_objects, LIBS=esp_libs)

#==============UTILITY PROGRAMS======================

sqlite_sources = uplift_env.FilteredGlob("#/src/sqlite3/*.c", ["shell.c"])
sqlite_objects = uplift_env.Object(sqlite_sources)

sqlite_shell_object = uplift_env.Object(["#/src/sqlite3/shell.c"])
sqlite_libs = esp_libs
if build_platform == "posix":
    sqlite_libs = sqlite_libs + ['readline']
sqlite_shell = uplift_env.Program("sqlite", sqlite_objects + sqlite_shell_object, LIBS=sqlite_libs)

uplift_sources = uplift_env.Glob("#/src/uplift/*.cpp")
uplift_objects = uplift_env.Object(uplift_sources)

esp_uplift = uplift_env.Program("esp_uplift", sqlite_objects + uplift_objects, LIBS=esp_libs)

#===================TESTS=================
if build_tests:
    test_libs = esp_libs
    test_env = esp_env.Clone()
    
    Export(['test_env', 'test_libs', 'esp_lib'])
    test_scripts = esp_env.Glob("#tests/*.scons")
    for s in test_scripts:
        rval = SConscript(s)
        Depends(rval, esp_lib)




