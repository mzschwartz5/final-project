# Node based SDF Character Modeler

*A bespoke OpenGL application for creating 3D models via a node-based editor and L-system like commands that generate and blend SDFs.*

![FinalResult](/img/final_result.webp)

![Bear Demo](/img/beardemo.gif)

![Demo](/img/demo.gif)

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

(Not included: my initial attempts to integrate this as a blender plugin and use the blender python API to generate geometry. Also: forays into marching cubes to generate a polygonal mesh out of SDFs)

# SDFs

SDFs, or Signed distance fields, are simply functions which return the distance to a surface (in 3D), given some point in space. The simplest SDF is likely the sphere, whose function is simply:

```GLSL
float sphereSDF(in vec3 point, in vec4 sphere) {
    return length(point - sphere.xyz) - sphere.w; // where sphere.w is the radius
}
```

That one-line function tells us how close we are to the sphere. When it returns 0, we know we're at its surface. This property rather useful for a technique called raymarching, where we "shoot" rays out from a virtual camera towards objects in our scene, marching along each ray until we hit something, and then draw what we hit. SDFs can tell us whether a ray "hit" an object, and, if we miss, can tell us how big our next step along the ray should be to quickly converge on our sphere.

But perhaps the more useful property of SDFs, for the purposes of this project, is that two or more can be blended together simply by taking the minima of their values:

```GLSL
float blendedSDF = min(sdfA, sdfB);
```

And for a smooth blend between two or more SDFs, you can offset the minimum by a quadratic function:

```GLSL
float smoothMin( float sdfA, float sdfB, float k )
{
    k *= 4.0;
    float h = max( k - abs(sdfA - sdfB), 0.0 ) / k;
    return min(sdfA, sdfB) - h * h * k * (1.0 / 4.0);
}
```

For a lot more information on smooth minima and SDFs, see Inigo Quilez's work [here](https://iquilezles.org/articles/).

For this project however, it suffices to say that we can create smooth joins in our geometry as our turtle marches along by blending successive SDFs via a quadratic smooth minimum. It comes with a major downside, however: changes are not local. Any time new geometry is added, all previous geometry can be affected, sometimes strongly. Even simply putting two identical SDFs in the same location yields unintuitive results, when blended: instead of simply obscuring each other, the two SDFs will actually grow in size. It makes modeling quite unwieldy. As I discuss in the future directions section below, there are certainly methods for avoiding this (such as spatial partitioning), and methods that don't involve SDFs at all, but these were out of scope of my project.

Lastly, I actually used SDF ellipsoids rather than spheres because ellipsoids have three radii which I used as multi-directional "scale" quantities as the turtle marches along.

# Raymarching vs. Marching cubes (and others)

I discussed briefly how raymarching works. But now I want to discuss the pros and cons of raymarching when compared with other methods like marching cubes.

Pros:
- Relatively easy to implement
- Inherently parallel
- Yields high visual fidelity results

Cons:
- Doesn't actually generate a mesh, just shades pixels.
- Similarly, once the process is done, you can't actually do anything with the results before they're rendered.
- Has to run every frame

I went with ray marching largely for pros #1, #2, and #4. But initially, I started out with another method: marching cubes. This algorithm (famously the most cited paper in graphics) takes a scalar field and turns it into a mesh. It does so by evaluating each vertex of each cube in a grid and drawing triangles between the cube's edges depending on which vertices are above or below a so-called "isolevel" of the scalar field.

So, since SDFs, as implicit surfaces, can be thought of as scalar fields, marching cubes can be used to turn them into triangular meshes and render those triangles! These are my pros and cons for marching cubes:

Pros:
- Simple algorithm, easy to implement on the CPU.
- Run once (or any time SDFs change), then simply render the triangles every frame.
- Produces a mesh!

Cons:
- Low resolution results, some artifacts; higher resolution requires a lot more computation.
- Somewhat difficult to parallelize (lots of research here)

In terms of the point about visual fidelity, just take a look at this SDF sphere pair rendered via marching cubes:

![SDFs rendered via marching cubes](/img/HelloMetaballs.png)

And that actually took my poor computer a good 10-15 seconds to generate the mesh (admittedly, with a very naive MC implementation with high vertex duplication).

Nonetheless, I do actually think that marching cubes (or derivatives thereof, such as dual contouring) may prove the optimal way to proceed in the future of this project, for the reasons stated in the pros above. Most notably, to do anything interesting with procedural characters, such as skinning, rigging, and animating them, you have to have a mesh! But perhaps raymarching could still be used for high fidelity previews during editing, before baking into a mesh!

# Bytecode interpreter

Lastly, I'd like to talk a little bit about this "bytecode interpreter" (see the attributions below for my reference on this). I think Bob Nystrom really says it best with his tagline: *"Give behavior the flexibility of data by encoding it as instructions for a virtual machine."* It's a pattern used to give sandboxed control to the user over your application, by giving them a series of commands they can issue to your system, each of which affects some piece of the system's state. There are two keys here:

1. You, the developer, get to decide which pieces of state the user can control.
2. The user can influence the application state without having to write any code.

Now, that second one isn't too special on its own: we influence application all the time without code by clicking and using key inputs. What makes this pattern special is that, by encoding commands as data (at its simplest, integers), we can easily save, load, modify, and share how we influenced the application.

Putting this into the context of my SDF application:

The node system is a set of commands. The turtle is the interpreter, processing each node and setting state accordingly (the turtle's position, its orientation, the scale for the SDFs, etc.) By using this pattern, its trivial to save a 3D model; simply save the command nodes to a file, and when you reload them the next time you open the application (or if you send it to someone else), just reinterpret the nodes to regenerate the model. It's lightweight, fast, and easily extensible; want to implement a new feature? Just create a new node and have it interact with the turtle's state as needed.

## Node types

A list of node types currently implemented:
- Move - repositions the turtle (and the node that actually draws geometry!).
- Yaw - rotate the turtle in the yaw axis.
- Pitch - rotate the turtle in the pitch axis.
- Scale - scale the shape of the SDFs drawn in the XYZ directions.
- Store - save the transform of the turtle.
- Restore - set the turtle's transform back to the last stored transform (without drawing).
- SampleDensity - controls how many SDFs to place down between two points when the turtle moves.
- SampleBias - controls how attributes (scale) change along a path segment of the turtle. E.g. if the bias is 0.5, scale will change linearly from the turtle's starting position to end position.

## Future direction
- More SDF shapes!
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

## Bloopers

A few moments that tickled me, I hope you also find some joy from them!

![Bear monstrosity](/img/blooper.png)

<p align="center"><i>The bear shown in the title image with the k-value (SDF blend thickness turned down), oh bother! </i></p>


And a stack overflow that occurred *within* my bytecode interpreter... I have no one to blame but myself for this error!

![Stack overflow!](/img/StackOverflow.png)

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