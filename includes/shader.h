#ifndef SHADER_H
#define SHADER_H

typedef unsigned int Shader;

typedef enum {
    FALSE,
    TRUE,
} Bool;

Shader shaderNew(const char *, const char *);
void   shaderUse(Shader);
void   shaderSetBool(Shader, const char *, Bool);
void   shaderSetInt(Shader, const char *, int);
void   shaderSetFloat(Shader, const char *, float);

#endif