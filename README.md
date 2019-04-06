# Lasers and Bots

A simple game with lasers and bots

[![License: Apache2](https://img.shields.io/badge/license-Apache%202-blue.svg)](/LICENSE)

## info

This is an experimental game

# requirements

- C++ compiler for your platform (clang, gcc, etc.)
- C++ IDE (XCode, Visual Studio Express, etc.)
- python

## building

Init submodules

```bash
$ git submodule update --init
```

Update cocos2d-x files

```bash
$ cd cocos2d
$ git submodule update --init
$ python download-deps.py
```

Now you could open the project folder for the platform you like to use:

- proj.ios_mac
- proj.win32

## resources

- Game art : [Game Art 2D](https://www.gameart2d.com/)
- Game engine : [Cocox2d-x Github](https://github.com/cocos2d/cocos2d-x)
- JSON Parse : [Rapid JSON](https://github.com/Tencent/rapidjson)
- Map Editor : [Tiled](https://www.mapeditor.org/)
- Physics Editor : [Physics Body Editor](https://code.google.com/archive/p/box2d-editor/)
