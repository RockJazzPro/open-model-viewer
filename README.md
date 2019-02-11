# open-model-viewer
Simple little OpenGL model viewer written in C++.

## Motivation
This project is the result of a school project.

## Getting Started
### Prerequisites
You need [CMake](https://cmake.org) to build the project.
If you're not familiar with CMake, read the official [Documentation](https://cmake.org/documentation/).

### Installing
Clone this repository with submodules
```
git clone https://github.com/RockJazzPro/open-model-viewer.git
cd path/to/repo
git submodule init
git submodule update
```

Build it with CMake
```
mkdir build
cd build
cmake ..
```
> You can use the GUI if you're more familiar with it.

## Usage
* Look around with the `mouse`
* Move with `w`, `a`, `s` and `d`
* Enable the mesh view with `m`
* Export current scene as image with `e`
* Import another model with `ctrl` + `i`
* Import another model by dragging the model file (`.obj`) in the window


## Build With
* [GLFW](https://www.glfw.org/) - Windowing library
* [GLAD](https://glad.dav1d.de/) - OpenGL loading library
* [stb_image](https://github.com/nothings/stb) - Texture loading library
* [Assimp](http://www.assimp.org/) - Model loading library
* [GLM](https://glm.g-truc.net/0.9.9/index.html) - Math library
* [tinyfiledialogs](https://sourceforge.net/projects/tinyfiledialogs/) - File dialogs library

## License
This project is licensed unter the MIT License - see [LICENSE](LICENSE) file for details

## Acknowledgments
### Learning Sources
* [learnopengl](https://learnopengl.com) - mainly used to learn OpenGL
* [opengl-tutorial](http://www.opengl-tutorial.org) - used to deepen the understandment of OpenGL
### Support
* A big thanks to [Tibo Clausen](https://github.com/xenotrix1337) who made some code reviews.