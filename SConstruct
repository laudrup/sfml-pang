import platform

if platform.system() == "Linux":
    build_dir = "build/linux"
elif platform.system() == "Windows":
    build_dir = "build/windows"

AddOption('--builddir', dest="builddir", default=build_dir, help="Directory to build to.")
SConscript('SConscript', variant_dir=GetOption('builddir'), duplicate=0)
