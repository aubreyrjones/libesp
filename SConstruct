esp_env = Environment(tools=['default','textfile'], CPPPATH=['#/src'])
esp_env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME']=1

def filtered_glob(env, pattern, omit=[], ondisk=True, source=False, strings=False):
    return filter(lambda f: os.path.basename(f.path) not in omit, env.Glob(pattern))

def prefix_with(prefix):
    return lambda x: prefix + x

esp_env.AddMethod(filtered_glob, "FilteredGlob")

build_platform = None
build_64 = (ARGUMENTS.get('build_64', 'false') == 'true')
build_debug = (ARGUMENTS.get('debug', 'false') == 'true')

if esp_env['PLATFORM'] == 'posix':
    build_platform = 'posix'
elif esp_env['PLATFORM'] == 'win32':
    build_platform = 'win'
elif esp_env['PLATFORM'] == 'darwin':
    esp_env.Replace(CXX = "clang")
    build_64 = True
    build_platform = 'mac'

if build_platform == 'posix':
    esp_env.AppendUnique(CPPDEFINES = ['ESP_LINUX'])
    esp_env.AppendUnique(CCFLAGS = Split('-std=c++0x -fno-exceptions -fno-rtti'))
    esp_env.AppendUnique(LINKFLAGS = Split('-rdynamic -fno-exceptions -fno-rtti'))


esp_sources = esp_env.Glob("#/src/*.cpp")
esp_objects = esp_env.Object(esp_sources)

esp_shlib = esp_env.SharedLibrary("esp", esp_objects)

