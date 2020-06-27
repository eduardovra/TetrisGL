#ifndef _SHADER_H_
#define _SHADER_H_

char * read_file (const char *path);
unsigned int createShader (const char *shaderPath, GLuint shaderType);
unsigned int createProgram (const char *vertexShaderPath, const char *fragmentShaderPath);

#endif // _SHADER_H_
