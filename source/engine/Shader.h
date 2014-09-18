#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include <map>
#include <OpenGL/gl3.h>
#include "Math.h"

class Shader
{
  std::string m_vertexShaderFile, m_geometryShaderFile, m_fragmentShaderFile;

  std::string LoadFile(std::string n_path);
  void CheckShader(GLuint n_shader);
  void CheckProgram(GLuint n_program);

  GLuint m_program;

  std::map<std::string,GLint> m_variableToLocation;

public:
  Shader( std::string n_vertexShaderFile, std::string n_geometryShaderFile, std::string n_fragmentShaderFile );

  void Init();

  void Begin();
  void End();

  GLint Location(std::string n_variable);
  GLuint ID();


  // easy setter for ultralazy peoplez
  void SetMat(std::string n_matVariable, glm::mat4x4 n_mat);
  void SetTexture(std::string n_name, int loc);
  void SetFloat(std::string n_name, float n_float);
  void SetVec3(std::string n_name,float n_r, float n_g, float n_b);
  void SetVec3(std::string n_name,glm::vec3 n_vec);
};

class ShaderManager
{
  std::map<std::string, Shader*> m_map;
public:

  // adds a shader by name and shader source by relative to resource folder path
  // also returns newly created shader
  Shader* AddShader(std::string n_name, std::string n_vs, std::string n_gs, std::string n_fs);

  // the name must be relative to the resource path
  void AddShader(std::string n_name, Shader* n_shader);
  Shader* GetShader(std::string n_name);
};

extern ShaderManager g_shaderManager;

#endif
