# Features
- User Interface For Ease Of Use. 
- Multiple Shaders. 
- Particle Rendering System. 
- High Resolution Skybox.
- Low-poly Style 3D Environment. 

# Shaders
## PBR
Change the type and colour of the material. \
This shader uses multiple algorithms that create the correct lighting and physical conditions in order to correctly shade the object to be as realistic as possible.
## Wireframe
This shader display the triangles that makeup a model using a geometry shader. \ 
This shader can be modified using the in-game interface tools to display different colours and sizes
## Basic Textured
A basic shader is included to be able to display textures per-model. \ 
This allows for multiple objects in the scene at once. 
## Geometry Shaders
Geometry shaders take advantage of the power of a graphics card in order to create geometry. \
By passing data from the CPU to the GPU creating complex geometry is much faster and as a result is much more usable within a project.

# Models
## Setting Matrices
Each model requires a set of viewing matrices. \
These matrices are determined at runtime from the point of view of the camera and viewport.
## Rendering Meshes
Each mesh is rendered using the Mesh component.
## Full Enviroment
The full environment loops through the list of objects in the scene and renders them with the correct shaders.

# Textures
## Loading Textures
Textures are loaded at the start of the Scene and are then used throughout the rendering of the Scene.
## Rendering Textures
Rendering textures is done simply by binding the textures each time an object is rendered. \
This system can be improved by implementing a texture atlas and instead not having to keep rebinding textures.
# Interface (ImGui)
The ImGui interfact was made in order to be able to control what shaders are rendering the scene and how. \
The parameters can be changed to customise the scene.
# Keyboard and Mouse Interactivity
## Keyboard
The keyboard utilises a map of keys to check when a key is pressed. \
This map of keys is debounced by checking when the key was released each frame in the InputManager Update function. 
## Mouse
The mouse movement utilises the built in glfw mouse callback with a global impementation for determining camera movement in the InputManager object. 










