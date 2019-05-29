#/****************************************************************************
# Copyright (c) 2019 Juan Medina
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

# system imports
import logging
import os
import sys
from os import walk
import subprocess
import xml.etree.ElementTree as ET

# Log object
log = None

# variables
doc_type = '<?xml version="1.0" encoding="UTF-8"?>\n\
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">\n'


def write_plist(file_path, tree):
    xml_string = ET.tostring(tree.getroot()).decode('utf-8')

    with open(file_path, "w") as xf:
        xf.write(doc_type + xml_string)
        log.info("file written: %s", file_path)


def get_version(dom_tree):
    root = tree.getroot()
    root_dict = root.find("dict")
    key = root_dict.find("key")

    if key.text == "version":
        version_dict = root_dict.find("dict")
        version = dict()
        values = version_dict.findall("integer")
        version["major"] = int(values[0].text)
        version["minor"] = int(values[1].text)
        version["patch"] = int(values[2].text)
        version["build"] = int(values[3].text)
        return version


def read_plist(file_path):
    log.info("parsing: %s", file_path)
    tree = ET.parse(file_path)

    root = tree.getroot()
    root_dict = root.find("dict")
    key = root_dict.find("key")

    if key.text == "version":
        log.info("file has version")
        return tree
    else:
        return None


def print_version(version):
    log.info("  major = %d", version["major"])
    log.info("  minor = %d", version["minor"])
    log.info("  patch = %d", version["patch"])
    log.info("  build = %d", version["build"])


def increment_build(tree):
    root = tree.getroot()
    root_dict = root.find("dict")
    key = root_dict.find("key")

    if key.text == "version":
        version_dict = root_dict.find("dict")
        values = version_dict.findall("integer")
        values[3].text = str(int(values[3].text) + 1)
        log.info("build incremented to: %s", values[3].text)


if __name__ == '__main__':
    try:
        LOG_FORMAT = "%(asctime)s - %(name)s - %(levelname)s - %(message)s"
        logging.basicConfig(level=logging.INFO, format=LOG_FORMAT)

        log = logging.getLogger(__name__)

        if len(sys.argv) > 1:
          plist_path = sys.argv[1]
          tree = read_plist(plist_path)
          if not (tree is None):
              version = get_version(tree)
              print_version(version)
              if len(sys.argv) == 3:
                  if sys.argv[2] == "--increment-build":
                      increment_build(tree)
                      write_plist(plist_path, tree)
    except Exception as ex:
        logging.error(ex, exc_info=True)
        raise ex
