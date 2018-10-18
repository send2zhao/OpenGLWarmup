import os
import requests
import zipfile
import re

from os.path import splitext

DEST_FOLDER = 'repo'

# dependes:
# SDL2, glew, glm.

# WINDOWS VERSION
urls = [
    'https://www.libsdl.org/release/SDL2-devel-2.0.7-VC.zip',
    'https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0-win32.zip/download',
    'https://github.com/g-truc/glm/archive/0.9.8.5.zip'
]


def download_file(url, allow_redirects=True):

    # NOTE the stream=True parameter
    r = requests.get(url, stream=True, allow_redirects=True)
    if ('Content-Disposition' in r.headers.keys()):
        local_filename = re.findall("filename=(.+)", r.headers['Content-Disposition'])[0]
    else: 
        local_filename = r.url.split('/')[-1]
    print(r.url, local_filename)
    with open(local_filename, 'wb') as f:
        for chunk in r.iter_content(chunk_size=1024):
            if chunk:  # filter out keep-alive new chunks
                f.write(chunk)
                # f.flush() commented by recommendation from J.F.Sebastian
    return local_filename


def main():
    # main loop
    if (not os.path.exists(DEST_FOLDER)):
        os.makedir(DEST_FOLDER)

    for url in urls:
        local_filename = download_file(url)
        _, ext = splitext(local_filename)
        if (ext.lower() in ['.zip']):
            with zipfile.ZipFile(local_filename, 'r') as myzip:
                myzip.extractall(DEST_FOLDER)
            os.unlink(local_filename.lower())


if __name__ == "__main__":
    print('Current Version is worked for WINDOWS')
    main()
