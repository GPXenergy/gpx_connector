import os
import time
import re
from shutil import copyfile
Import('env')


def post_build(source, *args, **kwargs):
    firmware_version = env.GetProjectOption("firmware_version")
    program_name = 'gpx_connector_%s.dev%d' % (firmware_version, int(time.time()))
    src = str(source[0])
    build_dir = src.rstrip('firmware.bin')
    dst = '%s%s.bin' % (build_dir, program_name)

    pattern = re.compile("gpx_connector_.+\.bin")
    for filepath in os.listdir(build_dir):
        if pattern.match(filepath):
            print('removed old build: ', build_dir + filepath)
            os.remove(build_dir + filepath)
    if os.path.isfile(src):
        print('added build: ', dst)
        copyfile(src, dst)


env.AddPostAction('buildprog', post_build)
