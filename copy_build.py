import os
from shutil import copyfile
Import('env')


def post_build(source, *args, **kwargs):
    firmware_version = env.GetProjectOption("firmware_version")
    program_name = 'gpx_connector_%s' % firmware_version
    src = str(source[0])
    dst = './firmware/gpx_connector_v1/%s.bin' % program_name
    if os.path.isfile(src):
        copyfile(src, dst)
        print('Firmware has been added to', dst)


env.AddPostAction('buildprog', post_build)
