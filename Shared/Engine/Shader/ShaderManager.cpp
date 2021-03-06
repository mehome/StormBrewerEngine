
#include "Engine/EngineCommon.h"
#include "Engine/Shader/ShaderManager.h"
#include "Engine/Shader/DefaultShader.h"

ShaderManager g_ShaderManager;
PreMainCallList g_ShaderRegisterList;

void ShaderManager::Init()
{
  m_DefaultShader = MakeQuickShaderProgram(kDefaultVertexShader, kDefaultFragmentShader);

  m_DefaultScreenSpaceShader = MakeQuickShaderProgram(kDefaultVertexShader, kDefaultScreenSpaceFragmentShader);
  g_ShaderRegisterList.CallAll();
}

void ShaderManager::Shutdown()
{
  m_DefaultShader.Destroy();
  m_Shaders.clear();
}

ShaderProgram & ShaderManager::GetShader(uint32_t shader_name_hash)
{
  for (auto & elem : m_Shaders)
  {
    if (shader_name_hash == elem.m_ShaderNameHash)
    {
      return elem.m_Shader;
    }
  }

  return m_DefaultShader;
}

ShaderProgram & ShaderManager::GetDefaultWorldSpaceShader()
{
  return m_DefaultShader;
}

ShaderProgram & ShaderManager::GetDefaultScreenSpaceShader()
{
  return m_DefaultScreenSpaceShader;
}

void ShaderManager::RegisterShader(ShaderProgram && shader_prgram, czstr shader_name)
{
  m_Shaders.push_back(ShaderInfo{ std::move(shader_prgram), crc32(shader_name) });
  m_ShaderNames.emplace_back(shader_name);
}

void ShaderManager::RegisterShader(czstr vertex_program, czstr fragment_program, czstr shader_name)
{
  RegisterShader(MakeQuickShaderProgram(vertex_program, fragment_program), shader_name);
}

void ShaderManager::RegisterShader(czstr fragment_program, czstr shader_name)
{
  RegisterShader(MakeQuickShaderProgram(kDefaultVertexShader, fragment_program), shader_name);
}

const std::vector<std::string> & ShaderManager::GetShaderNames() const
{
  return m_ShaderNames;
}
