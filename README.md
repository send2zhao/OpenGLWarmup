README
======
2018.10.15

This projection started from `https://github.com/BennyQBD/ModernOpenGLTutorial`.


OpenGL Test Project. Very simple code. Just to faimilar with basic concepts.


Dependencies
------------

Depends on following packages:

* SDL2 (SDL2-2.0.7) used to create the context.
    * two libs need to be linked: `sdl2.lib` and `sdl2main.lib`.
* GLEW (glew-2.1.0) The OpenGL Extension Wrangler Library. Get all the gl headers
  from this libaray. There are two libs, `glew32.lib` and `glew32s.lib`, where
  `glew32s.lib` is the static linked library. If use static lib, define `GLEW_STATIC`
  variable.
* glm (glm-0.9.8.5) OpenGL math library. All headers.
* imgui(included): Dear ImGui: Bloat-free Immediate Mode Graphical User interface for C++ with
  minimal dependencies. Just need a few files. This package is included in the `res`
  folder. Need to define `IMGUI_IMPL_OPENGL_LOADER_GLEW`.
* stb_image small foot-print image load/saving libraray. Included. This package is 
  inside `res` folder.


To install the dependencies. Run the `setup.py` script.
```
python setup.py
```

Work Environments
-----------------

First version of this toy project is on Ubuntu using CodeBlock. Later Visual Studio
is used. Currently (2018-10-15), only the VS is working. Need to fix the CodeBlock
environment.

