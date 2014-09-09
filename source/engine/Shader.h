#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include <map>
#include <OpenGL/gl.h>
#include "Math.h"

class Shader
{
  std::string m_vertexShaderFile, m_geometryShaderFile, m_fragmentShaderFile;

  std::string LoadFile(std::string n_path);
  void CheckShader(GLuint n_shader);
  void CheckProgram(GLuint n_program);

  GLuint m_program;

  std::map<std::string,GLuint> m_variableToLocation;

public:
  Shader( std::string n_vertexShaderFile, std::string n_geometryShaderFile, std::string n_fragmentShaderFile );

  void Init();

  void Begin();
  void End();

  GLuint Location(std::string n_variable);
  GLuint ID();


  // easy setter for ultralazy peoplez
  void SetMat(std::string n_matVariable, glm::mat4x4 n_mat);
};

#endif
