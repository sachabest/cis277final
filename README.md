## CIS 277 Final Project

### Milestone 2

#### Responsibilities

* Sacha: Terrain generation. continuous smoothed functions, efficient storage
    * Classes: Terrain, Point, Point3, parts of Scene
* Carrie:
	* Classes: Ray, Intersection, Cross, parts of MyGL, ShaderProgram, OctNode, Scene, vertex and fragment shaders

#### Animation is fully working 
#### Gravity is activated through pressing key "g"
#### Add and Delete Blocks is fixed - precise accuracy now.
		   
* Carolina: Chunk optimization, terrain shifting, octree, image file as heightmap
	* Classes: Chunk, OctNode, parts of Scene and MyGL

#### Chunks
Each world block is part of a 16x16x16 chunk VBO. When the world is initially loaded, it is only a 5x5 space (in terms of chunks), but as the player moves in any direction, new chunks are generated. Chunks more than 32 taxicab units away are not rendered.

#### Octree
Octree is fully implemented and greatly improves rendering speed. Its dimensions are 64x64x64 (in terms of chunks). Each time a chunk is created, it is automatically added to the octree.

#### Image file as heightmap
Click "Load Heightmap" and select one of the perlin noise PNG image files to load it into the game. To spawn the corresponding terrain at the user's current position, press C.
Some chunks that are regenerated and outside of the 5x5 space around the current user position may disappear, but will be re-rendered once the user approaches.
