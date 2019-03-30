from PIL import Image
import os
import logging
import shutil
import stat
from os import walk

# Log object
log = None


# Get file with a extension
def get_files(path, extension):
    log.info("reading folder [%s]", path)
    files = []
    fullpath = ""
    ext = ""

    for (dirpath, dirnames, filenames) in walk(path):
        for filename in filenames:
            fullpath = dirpath + os.sep + filename
            ext = os.path.splitext(os.path.basename(fullpath))[1]
            if (ext == extension):
                files.append(fullpath)

    return files


def join_images(folder, outFile):

    if os.path.isfile(outFile):
        os.remove(outFile)
        log.warning("deleting file %s", outFile)

    files = get_files(folder, ".png")
    num_files = len(files)

    im = Image.open(files[0])
    img_width, img_height = im.size

    cols = 2048 / img_width
    rows = num_files / cols

    if rows == 0:
        rows = 1
        cols = num_files

    width = img_width * cols
    height = img_height * rows

    img = Image.new('RGBA', (width, height), 255)
    row = 0
    col = 0
    for file in files:
        im = Image.open(file)
        img.paste(im, (col * img_width, row * img_height))
        col = col + 1
        if col > cols:
            row = row + 1
            col = 0
        log.info("processing : %s", file)

    img.save(outFile)
    log.info("file saved : %s", outFile)


if __name__ == '__main__':

    try:
        LOG_FORMAT = "%(asctime)s - %(name)s - %(levelname)s - %(message)s"
        logging.basicConfig(level=logging.INFO, format=LOG_FORMAT)

        log = logging.getLogger(__name__)

        outFile = "tilesheet.png"
        folder = "tiles"
        join_images(folder, outFile)

    except Exception as ex:
        logging.error(ex, exc_info=True)
        raise ex
