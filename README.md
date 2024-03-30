# 3D Engine Homebrew.

This project is a implementation of engine using OpenGL as GPU API.

Basic features:

- 3D/2D perspectives;
- "dynamic lighting" by shaders;
- Support multiple types of light: Directional, Point, Flashlight;
- Load .obj and others formats;
- Stencil testing; (TODO)
- Cubemaps aka skybox; (TODO)
- HDR; (TODO)
- Bloom... other; (TODO)
- Skeleton animation; (PROGRESS)
- Basic Physics engine; (TODO)
- Basic logic game/scenario/menu/UI; (TODO)

Future uses:

- Mesh map generator to be used as graphic demo; (TODO)
- Future homebrew games;

## How to build

Do you need: OpenGL devel;

```
cmake .
make run
```

## References
- ["The Cherno OpenGL Tutorial"](https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2)
- [OpenGL tutorial](https://learnopengl.com/)
- [LearOpenGL Repo](https://github.com/JoeyDeVries/LearnOpenGL)
- [STB lib](https://github.com/nothings/stb)
- [ImGui](https://github.com/ocornut/imgui)
- [ImGui Online Demo Doc](https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html)
- [Assimp](https://github.com/assimp/)
- [OBJ-Loader](https://github.com/Bly7/OBJ-Loader)
- [GLFW](https://www.glfw.org/)
- [GLM](http://glm.g-truc.net/0.9.8/index.html)
- [Wavefront .obj file](https://en.wikipedia.org/wiki/Wavefront_.obj_file)
- [OpenGL Documentation Reference](https://registry.khronos.org/OpenGL-Refpages/gl4/)
- [C++ RAII](https://en.cppreference.com/w/cpp/language/raii)
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
- [IEEE Arithmetic ](https://docs.oracle.com/cd/E19957-01/806-3568/ncg_math.html)
- [Improved Alpha-Tested Magnification for Vector Textures and Special Effects](https://steamcdn-a.akamaihd.net/apps/valve/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf)
- [Getting The Game Loop Right - Video about deterministic game loop](https://www.youtube.com/watch?v=lW6ZtvQVzyg)
- Block-diagram-of-the-OpenGL-pipeline

![Block-diagram-of-the-OpenGL-pipeline](https://github.com/thiagolopes/3d_engine_homebrew/assets/5994972/a5519698-8450-41e6-8db8-2e0b091be5b2)


## Screenshot
2D:
![2d](https://github.com/thiagolopes/3d_engine_homebrew/assets/5994972/d5368333-e06c-4b05-836b-d4904531e0e6)

3D:
![rock_with_light](https://github.com/thiagolopes/3d_engine_homebrew/assets/5994972/6d247aaa-2727-42f4-a343-6af49f26805b)
