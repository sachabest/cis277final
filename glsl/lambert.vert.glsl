#version 150
// ^ Change this to version 130 if you have compatibility issues

// This is a vertex shader. While it is called a "shader" due to outdated conventions, this file
// is used to apply matrix transformations to the arrays of vertex data passed to it.
// Since this code is run on your GPU, each vertex is transformed simultaneously.
// If it were run on your CPU, each vertex would have to be processed in a FOR loop, one at a time.
// This simultaneous transformation allows your program to run much faster, especially when rendering
// geometry with millions of vertices.

uniform mat4 u_Model;       // The matrix that defines the transformation of the
                            // object we're rendering. In this assignment,
                            // this will be the result of traversing your scene graph.

uniform mat4 u_ModelInvTr;  // The inverse transpose of the model matrix.
                            // This allows us to transform the object's normals properly
                            // if the object has been non-uniformly scaled.

uniform mat4 u_ViewProj;    // The matrix that defines the camera's transformation.
                            // We've written a static matrix for you to use for HW2,
                            // but in HW3 you'll have to generate one yourself

uniform int timer;

in vec3 vs_Pos;  // ---------->The array of vertex positions passed to the shader

in vec3 vs_Nor;  // ---------->The array of vertex normals passed to the shader

in vec3 vs_Col;  // ---------->The array of vertex colors passed to the shader.

in vec4 vs_uv;

out vec3 fs_Nor;  // --------->The array of normals that has been transformed by u_ModelInvTr. This is implicitly passed to the fragment shader.
out vec3 fs_LightVec;  // ---->The direction in which our virtual light lies, relative to each vertex. This is implicitly passed to the fragment shader.
out vec3 fs_Col;  // --------->The color of each vertex. This is implicitly passed to the fragment shader.
out vec4 fs_uv;
//just out = in here; give it to frag shader



const vec4 lightDir = vec4(1,1,1,0);  // The position of our virtual light, which is used to compute the shading of
                                        // the geometry in the fragment shader.

void main()
{
    fs_Col = vs_Col;  //                          Pass the vertex color positions to the fragment shader
    fs_Nor = vec3(u_ModelInvTr * vec4(vs_Nor, 0));  //           Transform the geometry's normals

    //3rd value is 1 = animation;
    if (vs_uv.b == 1) {
        //no offset
        if (timer == 0) {
            fs_uv = vs_uv;
        }
        //offset = 1
        else if (timer == 1) {
            fs_uv = vec4(vs_uv.x + 0.3/16.f, vs_uv.y, vs_uv.z, vs_uv.w);
            //fs_uv = vec4(1,0,0,0);
        }

        //offset = 2
        else if (timer == 2) {
            fs_uv = vec4(vs_uv.x + 0.6/16.f, vs_uv.y, vs_uv.z, vs_uv.w);
            //fs_uv = vec4(0,1,0,0);
        }

        //offset = 3
        else if (timer == 3) {
            fs_uv = vec4(vs_uv.x + 0.9/16.f, vs_uv.y, vs_uv.z, vs_uv.w);
            //fs_uv = vec4(0,0,1,0);
        }

        //offset = 4
        else if (timer == 4) {
            fs_uv = vec4(vs_uv.x + 1/16.f, vs_uv.y, vs_uv.z, vs_uv.w);
            //fs_uv = vec4(1,1,1,0);
        }
    }

    //no animation; out = in
    else {
        fs_uv = vs_uv; // out uv = in uv
        //fs_uv = vec4(1,0,0,0);
    }
    //fs_uv = vs_uv;

    vec4 modelposition = u_Model * vec4(vs_Pos, 1);  //    Temporarily store the transformed vertex positions for use below

    fs_LightVec = (lightDir).xyz;  //   Compute the direction in which the light source lies

    gl_Position = u_ViewProj * modelposition;  // gl_Position is a built-in variable of OpenGL which is used to render the final positions
                                             // of the geometry's vertices
}
