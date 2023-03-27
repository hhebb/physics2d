import gymnasium as gym
import envs
import numpy as np
import OpenGL
import glfw
import glm
from OpenGL.GL import *
from glfw.GLFW import *

def load_shaders(vertex_shader_source, fragment_shader_source):
    # vertex shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER) # create an empty shader object
    glShaderSource(vertex_shader, vertex_shader_source) # provide shader source code
    glCompileShader(vertex_shader) # compile the shader object
    
    # check for shader compile errors
    success = glGetShaderiv(vertex_shader, GL_COMPILE_STATUS)
    if (not success):
        infoLog = glGetShaderInfoLog(vertex_shader)
        print("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + infoLog.decode())
    
    # fragment shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER) # create an empty shader object
    glShaderSource(fragment_shader, fragment_shader_source) # provide shader source code
    glCompileShader(fragment_shader) # compile the shader object
    
    # check for shader compile errors
    success = glGetShaderiv(fragment_shader, GL_COMPILE_STATUS)
    if (not success):
        infoLog = glGetShaderInfoLog(fragment_shader)
        print("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + infoLog.decode())
    
    # link shaders
    shader_program = glCreateProgram() # create an empty program object
    glAttachShader(shader_program, vertex_shader) # attach the shader objects to the program object
    glAttachShader(shader_program, fragment_shader)
    glLinkProgram(shader_program) # link the program object
    
    # check for linking errors
    success = glGetProgramiv(shader_program, GL_LINK_STATUS)
    if (not success):
        infoLog = glGetProgramInfoLog(shader_program)
        print("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + infoLog.decode())
    
    glDeleteShader(vertex_shader)
    glDeleteShader(fragment_shader)
    
    return shader_program # return the shader programam


def key_callback(window, key, scancode, action, mods):
    if key==GLFW_KEY_ESCAPE and action==GLFW_PRESS:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    elif key==GLFW_KEY_A:
        if action==GLFW_PRESS:
            print('press a')
        elif action==GLFW_RELEASE:
            print('release a')
    elif action==GLFW_REPEAT:
        print('repeat a')
    elif key==GLFW_KEY_SPACE and action==GLFW_PRESS:
        print ('press space: (%d, %d)'%glfwGetCursorPos(window))



g_vertex_shader_src = '''
#version 330 core
// input vertex position. its attribute index is 0.
layout (location = 0) in vec3 vin_pos; 
void main()
{
    // gl_Position: built-in output variable of type vec4 to which vertex position in clip space is assigned.
    gl_Position = vec4(vin_pos.x, vin_pos.y, vin_pos.z, 1.0);
    // gl_Position.xyz = vin_pos;
    // gl_Position.w = 1.0;
}
'''

g_fragment_shader_src = '''
#version 330 core
// output fragment color of type vec4.
out vec4 FragColor;
void main()
{
    // set the fragment color to white.
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
'''


def main():
    # initialize glfw
    if not glfwInit():
        return
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3) # OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE) # Do not allow legacy OpenGl API calls
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE) # for macOS
    
    # create a window and OpenGL context
    window = glfwCreateWindow(800, 800, '1-first-gl-program', None, None)
    if not window:
        glfwTerminate()
        return
    
    glfwMakeContextCurrent(window)
    
    # register key callback for escape key
    glfwSetKeyCallback(window, key_callback);
    # load shaders
    shader_program = load_shaders(g_vertex_shader_src, g_fragment_shader_src)
    
    # create and activate VAO (vertex array object)
    VAO = glGenVertexArrays(1)
    glBindVertexArray(VAO)
    
    # create and activate VBO (vertex buffer object)
    VBO = glGenBuffers(1)
    glBindBuffer(GL_ARRAY_BUFFER, VBO)
    
    # copy vertex data to VBO
    glBufferData(GL_ARRAY_BUFFER, vertices.nbytes, vertices.ptr, GL_STATIC_DRAW)
    
    # configure vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * glm.sizeof(glm.float32), None)
    glEnableVertexAttribArray(0)
    
    # loop until the user closes the window
    while not glfwWindowShouldClose(window):
        # get vertices
        a = -1.0 if np.random.random() > .5 else 1.0
        s, r, d, info = env.step([a])
        objs = env.environment.GetEnvRenderVertices()

        
        for obj in objs:
            poly = print(len(obj)//3)

        vertices = np.array([c for obj in objs for c in obj])
        
        # render
        glBufferData(GL_ARRAY_BUFFER, vertices.nbytes, vertices, GL_STATIC_DRAW)
        glClear(GL_COLOR_BUFFER_BIT)
        glUseProgram(shader_program)
        glBindVertexArray(VAO)

        offset = 0
        for obj in objs:
            n_poly = len(obj)//3
            if n_poly == 3:
                poly_name = GL_TRIANGLES
            elif n_poly == 4:
                poly_name = GL_QUADS

            print(n_poly)
            glDrawArrays(poly_name, offset, n_poly)
            offset += n_poly

        # swap front and back buffers
        glfwSwapBuffers(window)

        # poll events
        glfwPollEvents()

    # terminate glfw
    glfwTerminate()


if __name__ == "__main__":
    env = gym.make('envs/cartpole-v0')
    s, info = env.reset()
    main()