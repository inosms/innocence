#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Error.h"
#include "Math.h"

// http://content.gpwiki.org/index.php/OpenGL:Tutorials:Loading_and_using_GLSL_shaders
// https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/loading.php

Shader::Shader( std::string n_vertexShaderFile, std::string n_geometryShaderFile, std::string n_fragmentShaderFile ) :
  m_vertexShaderFile(n_vertexShaderFile),
  m_geometryShaderFile(n_geometryShaderFile),
  m_fragmentShaderFile(n_fragmentShaderFile)
{
  INFO("shading language version " << glGetString(GL_SHADING_LANGUAGE_VERSION));
}


std::string Shader::LoadFile(std::string n_path)
{
  // then no file was specified
  if( n_path.length() == 0 ) return "";

  std::fstream tmp_inputFile(n_path,std::ios::in);

  if( tmp_inputFile.is_open() )
  {
    std::stringstream tmp_buffer;
    tmp_buffer << tmp_inputFile.rdbuf();
    return tmp_buffer.str();
  }
  else
    throw new std::string("file not found");
}

void Shader::CheckShader(GLuint n_shader)
{
  GLint tmp_hasCompiled;

  glGetShaderiv(n_shader, GL_COMPILE_STATUS, &tmp_hasCompiled);
  if( !tmp_hasCompiled )
  {
    // get length of info buffer
    GLint tmp_lengthOfInfoLog;
    glGetShaderiv(n_shader, GL_INFO_LOG_LENGTH, &tmp_lengthOfInfoLog );

    // create temporary buffer for info log
    std::vector<char> tmp_logBuffer;
    tmp_logBuffer.resize(tmp_lengthOfInfoLog);

    // get the info log
    GLint tmp_finalLength;
    glGetShaderInfoLog(n_shader,tmp_lengthOfInfoLog,&tmp_finalLength,&tmp_logBuffer[0]);

    std::string tmp_finalLog(&tmp_logBuffer[0],tmp_finalLength);
    ERROR_MESSAGE(tmp_finalLog);

    throw new std::string("failed to compile shader");
    // TODO: find out the concrete error
  }
}

void CheckProgram(GLuint n_program)
{
  GLint tmp_hasLinked;
  glGetProgramiv(n_program, GL_LINK_STATUS, &tmp_hasLinked);
  if(!tmp_hasLinked)
  {
     throw new std::string("failed to link shader program");
     // TODO: again find out the log
  }
}

void Shader::Init()
{
  std::string tmp_vertexShaderSourceCode    = "";
  std::string tmp_geometryShaderSourceCode  = "";
  std::string tmp_fragmentShaderSourceCode  = "";

  try
  {
    tmp_vertexShaderSourceCode = LoadFile( m_vertexShaderFile );
  }
  catch(std::string &e){ ERROR_MESSAGE(e);}

  try
  {
    tmp_geometryShaderSourceCode = LoadFile( m_geometryShaderFile );
  }
  catch(std::string &e){ ERROR_MESSAGE(e);}

  try
  {
    tmp_fragmentShaderSourceCode = LoadFile( m_fragmentShaderFile );
  }
  catch(std::string &e){ ERROR_MESSAGE(e);}

  // TODO: geometry shader
  GLuint tmp_vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
  GLuint tmp_fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

  // attach shader source code to shader
  // https://stackoverflow.com/questions/6047527/how-to-convert-stdstring-to-const-char
  const char *c_str = tmp_vertexShaderSourceCode.c_str();
  glShaderSource(tmp_vertexShaderObject,1,&c_str,NULL);
  c_str = tmp_fragmentShaderSourceCode.c_str();
  glShaderSource(tmp_fragmentShaderObject,1,&c_str,NULL);

  glCompileShader(tmp_vertexShaderObject);
  glCompileShader(tmp_fragmentShaderObject);

  // now check for errors:
  CheckShader(tmp_vertexShaderObject);
  CheckShader(tmp_fragmentShaderObject);

  m_program = glCreateProgram();

  glAttachShader(m_program,tmp_vertexShaderObject);
  glAttachShader(m_program,tmp_fragmentShaderObject);

  glLinkProgram(m_program);
}

// TODO: maybe a stack to push and pop?
void Shader::Begin()
{
  glUseProgram(m_program);
}

void Shader::End()
{
  glUseProgram(0);
}

GLuint Shader::Location(std::string n_variable)
{
  if( m_variableToLocation.find(n_variable) == m_variableToLocation.end() )
  {
    m_variableToLocation[n_variable] = glGetUniformLocation(m_program,n_variable.c_str());
  } 

  return m_variableToLocation[n_variable];
}

GLuint Shader::ID()
{
  return m_program;
}

void Shader::SetMat(std::string n_matVariable, glm::mat4x4 n_mat)
{
  glUniformMatrix4fv(Location(n_matVariable),1,GL_FALSE,&n_mat[0][0]);
}

