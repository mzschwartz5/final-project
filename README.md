# Node based SDF Character Modeler

![FinalResult](/img/final_result.webp)

# Introduction

This project started with a vision for game where players create and modify procedural characters as necessary to solve various puzzles. There are a handful of ways to create procedural organisms; famously, L-systems are used to generate procedural plants. And one could craft a system to generate a *particular type* of organism (say a fish) with varying attributes (scale density, body shape, etc). To be able to generate *any* type of organism, on the other hand... that necessitates giving the user a high degree of control over the modeling process. However, as you give the user more and more control, you end up with a 3D modeling application like Blender, Maya, or ZBrush; powerful tools, but not games by most definitions, and too complex for the average user.

So I sought to strike a balance between complexity of the character creation tool, and the utility of the tool. A tool that any*body* can use to create any*thing*. What I landed on draws from procedural techniques such as SDFs and L-systems, but presents them as drag-and-drop nodes that can be easily wired together to create a 3D character model.

# The Main Idea

Imagine a turtle (the usual suspect in L-systems). When it moves, it draws a line in space behind it. The turtle can turn left and right (`yaw`) and tilt up and down (`pitch`). It can remember previous positions and teleport back there. With these simple commands, you can theoretically draw any picture in 2D or 3D space; but only out of lines.

![Turtle draws lines](/img/HelloTurtle.png)
<p align="center"><i>The turtle is instructed to remember its position, move forward, return, rotate, and repeat 8 times to draw a star.</i></p>

But what if, instead of lines, the turtle drew "tubes" behind it as it moved? And, if in addition to moving, turning, and teleporting, the turtle could also control the various directional scales of those tubes as it travels, you could create a 3D model:

![Turtle draws tubes](/img/MatCapRaymarchedEllipsoidSDFs.png)
<p align="center"><i>The turtle now draws tubes as it goes (composed of SDFs, described more below)</i></p>


## Limitations and caveats

The proposed method, so far, only allows for additive geometry. Subtractive *is* possible with SDFs, but not implemented in this project. Additionally, as you may have noticed (especially in the title image of the bear), the models created via this method are rather smooth and blobby. This is a result of a smoothing function applied between successive turtle steps among all SDF components. It is simultaneously one of the greatest pros and mischevious cons of the method

# Features / Components

Before discussing the processs of creating this tool and the techniques involved, I'd like to simply list some of the features and components of the project:
- OpenGL-based rendering pipeline
- SDF models with two rendering options: via raymarching and marching cubes
- Split-screen node-based editing UI and character model viewport
  - Split viewport with resizing and multiple cameras implemented natively in OpenGL
- ImGui- / ImNodes-based node editor with save-to- and load-from-file functionality (using Cereal for binary data serialization)
- Bytecode interpreter for translating turtle commands to geometry
- Various turtle command nodes, including:
  - Move, scale, number primitive, pitch and yaw, store and restore transform, sample density and sample bias nodes.
- Support for Windows and MacOS build with launch and task files for VSCode

# High-level Design Overview

![Flowchart design](/img/FlowChart2(1).svg)

(Not included: my initial attempts to integrate this as a blender plugin and use their API to generate geometry. Also: forays into marching cubes to generate a polygonal mesh out of SDFs)

## Future direction
- Spatial hierarchy for SDF blending so only affect each other locally. Perhaps even along direction of turtle / only neighboring joints can blend.
- Or abandon SDFs all together and use marching cubes with "paint brush" style (or terrain editor style) scalar field filling.
- More node types, possibly with control flow logic and more procedural options (noise, functions, time, color, bezier curves etc.)
- Meshify, skin, and rig the model.
- Lastly, and most importantly, many more UI and UX improvements... as a user of my own tool, boy does it suck to work with!


## Build and run
Note: changes to the project have made it incompatible with MacOS (which does not support SSBOs in OpenGL). The project can still be built and run in Windows by following the steps below.

- This project is set up to run on VSCode, but, with minor modifications, could be made to use MSVC and run in VS, or simply built and run via the command line.
- Create build folder under the project root directory.
- Note: the next step requires jinja2 to be installed (can be installed via pip or pip3)
  - The next step will also do a git submodule update to pull GLFW, Glad, and other dependencies.
  - Also, on Windows, you will need the G++ compiler and Ninja generator.
- Open the command pallette (Cmd (or Ctrl) + Shift + P) and run "Tasks: Run Build Task" and select Debug or Release. This will configure the project via CMake and build it.
- You can also run the executable in debug or releaase mode via the command pallette: Cmd (or Ctrl) + Shift + P > "Debug: Select and Start Debugging" (which will also configure and build the project first).
- You can also run "C/C++: Select a Configuration... > (Win32 | Mac)" to use Intellisense in VSCode.

- Alternatively, run the executable directly: `./Spliced` (or `./Spliced.exe` on Windows)

## Attributions:

- [Marching cubes python implementation](https://github.com/BorisTheBrave/mc-dc/blob/a165b326849d8814fb03c963ad33a9faf6cc6dea/marching_cubes_3d.py)

- [Ellipsoid SDF courtesy of IQ](https://iquilezles.org/articles/distfunctions/)

- [Smooth minimum SDF blending courtesy of IQ](https://iquilezles.org/articles/smin/)

- [Bytecode interpreter pattern, Bob Nystrom](https://gameprogrammingpatterns.com/bytecode.html)

- [LearnOpenGL for infrastructure setup](https://learnopengl.com/)

- UPenn CIS 4600/5600 slides and matcap materials

## Dependencies

### As git submodules
- glad
- glfw
- glm
- imgui
- stb
- cereal (used to serialize data for node editor save/load functionality)

### As header-only libraries
- imnodes (extension to imgui for node editor components)
- KHR