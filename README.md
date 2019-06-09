# Lasers and Bots

A game with lasers and bots

[![License: Apache2](https://img.shields.io/badge/license-Apache%202-blue.svg)](/LICENSE)

## info

This is an experimental open source game currently under development, it uses a 2d physics engine to implement a platformer.

## video demo
click the image bellow
<p align="center">
  <a href="https://www.youtube.com/watch?v=hrrWXePrd-0" target="_blank"><img src="https://img.youtube.com/vi/hrrWXePrd-0/maxresdefault.jpg" alt="video demo"></a>
</p>

## game controls

- Keyboard
  ```
  LEFT          : CURSOR: LEFT  / KEY: A
  RIGHT         : CURSOR: RIGHT / KEY: D
  JUMP          : CURSOR: UP    / KEY: W           / SPACE
  PAUSE         : F5
  ```
- Gamepad / Joystick
  ```
  LEFT          : D-PAD:  LEFT   / JOYSTICK: LEFT  / SHOULDER: LEFT
  RIGHT         : D-PAD:  RIGHT  / JOYSTICK: RIGHT / SHOULDER: RIGHT
  JUMP          : D-PAD:  UP     / JOYSTICK: UP    / BUTTON: A
  PAUSE         : BUTTON: START
  ```

## menu controls

- Keyboard
  ```
  LEFT          : CURSOR: LEFT  / KEY: A
  RIGHT         : CURSOR: RIGHT / KEY: D
  UP            : CURSOR: UP    / KEY: W
  DOWN          : CURSOR: DOWN  / KEY: S
  SELECT        : ENTER         / SPACE
  BACK          : ESCAPE
  ```
- Gamepad / Joystick
  ```
  LEFT          : D-PAD:  LEFT   / JOYSTICK: LEFT  / SHOULDER: LEFT
  RIGHT         : D-PAD:  RIGHT  / JOYSTICK: RIGHT / SHOULDER: RIGHT
  UP            : D-PAD:  UP     / JOYSTICK: UP
  DOWN          : D-PAD:  DOWN   / JOYSTICK: DOWN
  SELECT        : BUTTON: A
  BACK          : BUTTON: SELECT / BUTTON: B
  ```

## requirements

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
- proj.android

## resources

- Game art : [Game Art 2D](https://www.gameart2d.com/)
- Background : [MGG](https://mobilegamegraphics.com/product/free-parallax-backgrounds/)
- Map Editor : [Tiled](https://www.mapeditor.org/)
- Game engine : [Cocox2d-x](https://cocos2d-x.org/)
- Music:
  - Chasers (game) : [Juan Martorell](https://github.com/jmartorell)
  - Cellar (menu) : [Juan Martorell](https://github.com/jmartorell)
- SFX
  - Metal Footsteps : https://freesound.org/people/mypantsfelldown/sounds/398937/
  - Welding_2_longer_bursts : https://freesound.org/people/ivolipa/sounds/385990/
  - Squishy Boings : https://freesound.org/people/NoiseCollector/sounds/67403/
  - Level Failed : https://freesound.org/people/mouse85224/sounds/371205/
  - Victory : [Juan Martorell](https://github.com/jmartorell)
  - Star : [Juan Martorell](https://github.com/jmartorell)
  - Countdwn : https://freesound.org/people/tekgnosis/sounds/151268/
  - Explosion : https://freesound.org/people/plamdi1/sounds/95058/
