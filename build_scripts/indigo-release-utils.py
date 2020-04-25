import os
import shutil
import subprocess
import sys
from optparse import OptionParser
import re

version = ""
cur_dir = os.path.split(__file__)[0]
for line in open(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "api", "indigo-version.cmake")):
    m = re.search(r'SET\(INDIGO_VERSION "(.*)"', line)
    if m:
        version = m.group(1)


parser = OptionParser(description='Indigo utilities build script')
parser.add_option('--generator', help='this option is passed as -G option for cmake')
parser.add_option('--params', default="", help='additional build parameters')
parser.add_option('--config', default="Release", help='project configuration')
parser.add_option('--nobuild', default=False, action="store_true", help='configure without building', dest="nobuild")
parser.add_option('--clean', default=False, action="store_true", help='delete all the build data', dest="clean")
parser.add_option('--preset', type="choice", dest="preset", choices=list(irl.PRESETS.keys()), help='build preset %s' % (str(irl.PRESETS.keys())))

(args, left_args) = parser.parse_args()
if len(left_args) > 0:
    print("Unexpected arguments: %s" % (str(left_args)))
    exit()

_, args = irl.get_generator(args)

cur_dir = os.path.abspath(os.path.dirname(__file__))
root = os.path.normpath(os.path.join(cur_dir, ".."))
project_dir = os.path.join(cur_dir, "indigo-utils")

if args.generator.find("Unix Makefiles") != -1:
    args.params += " -DCMAKE_BUILD_TYPE=" + args.config

if args.preset and args.preset.find('universal') != -1:
    args.params += ' -DUNIVERSAL_BUILD=TRUE'

build_dir = (args.generator + " " + args.params)
build_dir = "indigo_utils_" + build_dir.replace(" ", "_").replace("=", "_").replace("-", "_")

full_build_dir = os.path.join(root, "build", build_dir)
if os.path.exists(full_build_dir) and args.clean:
    print("Removing previous project files")
    shutil.rmtree(full_build_dir)
if not os.path.exists(full_build_dir):
    os.makedirs(full_build_dir)

os.chdir(root)
if not os.path.exists("dist"):
    os.mkdir("dist")
dist_dir = os.path.join(root, "dist")

os.chdir(full_build_dir)
cmake_build_prefix = ''
if args.preset and (args.preset.find('linux') != -1 or args.preset.find('universal') != -1):
    cmake_build_prefix = 'CC=gcc CXX=g++'

args.params += ' -DWITH_STATIC=TRUE'

command = "%s cmake %s %s %s" % (cmake_build_prefix, '-G \"%s\"' if args.generator else '', args.params, project_dir)
print(command)
subprocess.check_call(command, shell=True)

if args.nobuild:
    exit(0)

for f in os.listdir(full_build_dir):
    path, ext = os.path.splitext(f)
    if ext == ".zip":
        os.remove(os.path.join(full_build_dir, f))

command = "cmake --build . --config %s" % (args.config)
print(command)
subprocess.call(command, shell=True)
if args.generator.find("Unix Makefiles") != -1:
    subprocess.check_call("make package", shell=True)
elif args.generator.find("Xcode") != -1:
    subprocess.check_call("cmake --build . --target package --config %s" % (args.config), shell=True)
elif args.generator.find("Visual Studio") != -1:
    subprocess.check_call("cmake --build . --target PACKAGE --config %s" % (args.config), shell=True)
elif args.generator.find("MinGW Makefiles") != -1:
    subprocess.check_call("mingw32-make package", shell=True)
else:
    print("Do not know how to run package and install target")
subprocess.check_call("ctest -V --timeout 10 -C %s ." % (args.config), shell=True)


for f in os.listdir(full_build_dir):
    path, ext = os.path.splitext(f)
    if ext == ".zip":
        shutil.copy(os.path.join(full_build_dir, f), os.path.join(dist_dir, f.replace('-shared', '')))
