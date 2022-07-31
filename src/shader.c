#include "../includes/shader.h"
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned int uint;

long get_file_size(FILE *fp)
{
    long length = 0;
    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return length;
}

Shader shaderNew(const char *vertexPath, const char *fragmentPath)
{
    // 1. retrieve the vertex / fragment source code from filePath
    char *vertexCode, *fragmentCode = NULL;
    long fsize = 0;

    FILE *vertexFile, *fragmentFile;
    vertexFile = fopen(vertexPath, "rb");
    if (vertexFile == NULL)
    {
        fprintf(stderr, "ERROR::SHADER::FAILED_TO_OPEN_FILE\n");
        exit(1);
    }
    fragmentFile = fopen(fragmentPath, "rb");
    if (fragmentFile == NULL)
    {
        fprintf(stderr, "ERROR::SHADER::FAILED_TO_OPEN_FILE\n");
        exit(1);
    }

    fsize = get_file_size(vertexFile);
    vertexCode = (char *)malloc(fsize + 1);
    if (vertexCode == NULL)
    {
        fprintf(stderr, "ERROR::SHADER::VERTEX::MEMORY_ALLOCATION_FAILED\n");
        exit(1);
    }
    fread(vertexCode, fsize, 1, vertexFile);
    vertexCode[fsize] = '\0';

    fsize = get_file_size(fragmentFile);
    fragmentCode = (char *)malloc(fsize + 1);
    if (fragmentCode == NULL)
    {
        fprintf(stderr, "ERROR::SHADER::VERTEX::MEMORY_ALLOCATION_FAILED\n");
        exit(1);
    }
    fread(fragmentCode, fsize, 1, fragmentFile);
    fragmentCode[fsize] = '\0';

    if (fclose(vertexFile) == EOF || fclose(fragmentFile) == EOF)
    {
        fprintf(stderr, "ERROR::SHADER::FAILED_TO_CLOSE_FILE\n");
    }

    // 2. compile shaders
    const char *vertexSource = vertexCode;
    const char *fragmentSource = fragmentCode;

    uint vertex, fragment;
    int success;
    char infoLog[512];

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }

    // shader program
    Shader shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex);
    glAttachShader(shader_program, fragment);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    free(vertexCode);
    free(fragmentCode);

    return shader_program;
}

void shaderUse(Shader shader)
{
    glUseProgram(shader);
}

void shaderSetBool(Shader shader, const char *name, Bool value)
{
    glUniform1i(glGetUniformLocation(shader, name), (int)value);
}

void shaderSetInt(Shader shader, const char *name, int value)
{
    glUniform1i(glGetUniformLocation(shader, name), value);
}

void shaderSetFloat(Shader shader, const char *name, float value)
{
    glUniform1f(glGetUniformLocation(shader, name), value);
}
