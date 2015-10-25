import platform

env = Environment(TARGET_ARCH="x86")

if platform.system() == "Linux":
    env.Append(CXXFLAGS=["-Wall", "-std=c++14", "-g"])
elif platform.system() == "Windows":
    env.Append(CPPPATH=["SFML/include", "Thor/include"],
               LIBPATH=["SFML/lib/", "Thor/lib"],
               CXXFLAGS=["-EHsc", "-Z7"])

SOURCES = ["main.cpp", "ball.cpp", "player.cpp", "shot.cpp", "background.cpp"]
LIBS = ["sfml-graphics", "sfml-window", "sfml-system", "thor"]

env.Program("test_sfml", SOURCES, LIBS=LIBS)
