from PIL import Image
import os
import logging
import json

# Log object
log = None


def process_physics_project(inFile, outFile):
    log.info("reading: %s ..", inFile)
    f = open(inFile, "r")
    if f.mode == 'r':
        contents = f.read()
        project = json.loads(contents)
        f.close()
        rigidBodies = project["rigidBodies"]
        for body in rigidBodies:
            log.info("body: %s", body["name"])
            imagePath = body["imagePath"]
            log.info("imagePath: %s", imagePath)
            imagePath = imagePath.replace("..", os.path.dirname(__file__))
            fn = os.path.join(os.path.dirname(__file__),
                              imagePath.replace("/", os.sep))
            log.info("fn: %s", fn)
            im = Image.open(fn)
            log.info("size: %dx%d", im.width, im.height)
            body["width"] = im.width
            body["height"] = im.height

    log.info("writing: %s ..", outFile)
    f = open(outFile, "w")
    if f.mode == 'w':
        f.write(json.dumps(project))
        f.close()
    return


if __name__ == '__main__':

    try:
        LOG_FORMAT = "%(asctime)s - %(name)s - %(levelname)s - %(message)s"
        logging.basicConfig(level=logging.INFO, format=LOG_FORMAT)

        log = logging.getLogger(__name__)

        inFile = os.path.join("shapes", "physics_project.json")
        outFile = os.path.join("shapes", "shapes.json")

        process_physics_project(inFile, outFile)

    except Exception as ex:
        logging.error(ex, exc_info=True)
        raise ex
