## CIS 277 Final Project

### Controls

* WASD: move (shift for extra speed)
* Arrows: rotate (shift for extra speed)
* 0, 8, 9: create foliage
* Q, E: Move down/up
* W, S: Zoom In/Out
* A, D: Move left/right
* G: enable gravity
* H: disable gravity
* R: remove block
* T: add block
* Shift + 1-7: change selected block type
* I: show/hide inventory

#### Responsibilities

* Sacha: Terrain generation and efficient storage, procedural foliage, HUD (inventory system + ui + sound effects)
* Carrie: Gravity + collisions + raymarching + octree intersection, texture mapping, animated textures
* Carolina: Chunk + world shifting, octree, loading image files as heightmaps

#### Terrain
Terrain is generated through a random perlin noise function, with caching for poins already visited that are no longer in the visible level (i.e. terrain you have visited persists). 

#### Foliage
Foliage is generated via the L-system standard and is stochastic in nature, though not fully working. 

#### HUD (inventory, ui, sound)
The app is a QGraphicsView containing a HUD widget and another QGraphicsView (MyGL) that represents the OpenGL scene. The HUD widget is a highly customized, transparent QWidget with a QListWidget holding hhe current inventory counts of all block types. You must remove blocks from the world of a type in order to place blocks of that type back in the world. 

#### Gravity and Collisions
Gravity is fully implemented. The user has a choice to either enable or disable gravity. The user's feet are firmly planted on the ground.

For collision, the user will not be able to move downwards once gravity is enabled (because you cannot go inside the world); when the user tries to jump gravity will bring the user down.

Collisions in the x and z directions are semi-working.

#### Raymarching and Octree Intersection
Raymarching is fully implemented. Octree intersection needs further tweets. Add/remove blocks uses raymarching to find the block to add/delete.

#### Texture mapping and Animated Textures
Each block in the world is mapped to a certain texture: STONE, LAVA, WATER, GRASS, and WOOD. Depending on the block's height in world position it has a certain height. Lava and water are animated; lava has an extra glow to it to simulate real lava.

#### Chunks
Each world block is part of a 16x16x16 chunk VBO. When the world is initially loaded, it is only a 5x5 space (in terms of chunks), but as the player moves in any direction, new chunks are generated. Chunks more than 32 taxicab units away are not rendered.

#### Octree
Octree is fully implemented and greatly improves rendering speed. Its dimensions are 64x64x64 (in terms of chunks). Each time a chunk is created, it is automatically added to the octree.

#### Image file as heightmap
Click "Load Heightmap" and select one of the perlin noise PNG image files to load it into the game. To spawn the corresponding terrain at the user's current position, press C.
Some chunks that are regenerated and outside of the 5x5 space around the current user position may disappear, but will be re-rendered once the user approaches.
