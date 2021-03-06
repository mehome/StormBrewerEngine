

#include "Foundation/Script/ScriptInterface.h"
#include "Foundation/Script/ScriptObject.h"
#include "Foundation/Script/ScriptState.h"
#include "Foundation/Script/ScriptInternal.h"

ScriptInterfaceCallback g_ScriptInterfaceCallbackFuncs[kScriptInterfaceMaxFunctions] = {};
ScriptInterfaceCallback g_ScriptDebugStubFuncs[kScriptInterfaceMaxFunctions] = {};

template <int Index>
struct ScriptInterfaceInitFunc
{
  static void Register()
  {
    g_ScriptInterfaceCallbackFuncs[Index] = [](lua_State * state) -> int
    {
      if(lua_istable(state, 1) == false)
      {
        ScriptFuncs::ReportError(state, "Invalid function call (use the colon operator)");
        return 0;
      }

      lua_pushstring(state, "ptr");
      lua_gettable(state, 1);
      auto interface_ptr = lua_touserdata(state, -1);
      lua_pop(state, 1);

      if(interface_ptr == nullptr)
      {
        ScriptFuncs::ReportError(state, "Couldn't read interface pointer");
        return 0;
      }

      auto interface = static_cast<ScriptInterface *>(interface_ptr);
      return (interface->m_ForwardDelegates[Index])(Index, interface, state);
    };

    g_ScriptDebugStubFuncs[Index] = [](lua_State * state) -> int
    {
      if(lua_istable(state, 1) == false)
      {
        ScriptFuncs::ReportError(state, "Invalid function call (use the colon operator)");
        return 0;
      }

      lua_pushstring(state, "ptr");
      lua_gettable(state, 1);
      auto interface_ptr = lua_touserdata(state, -1);
      lua_pop(state, 1);

      if(interface_ptr == nullptr)
      {
        ScriptFuncs::ReportError(state, "Couldn't read interface pointer");
        return 0;
      }

      auto interface = static_cast<ScriptInterface *>(interface_ptr);

      if(interface->m_DebugStubFunctionOutput[Index])
      {
        int top = lua_gettop(state);

        std::string message =
                interface->m_DebugStubFunctionNames[Index] + " Called With " + std::to_string(top - 1) + " Args";

        if (top >= 2)
        {
          message += ":\n";
        }

        for (int i = 2; i <= top; i++)
        {
          int t = lua_type(state, i);
          message += std::to_string(i - 1) + ". ";
          switch (t)
          {
            case LUA_TSTRING:  /* strings */
              message += lua_tostring(state, i);
              break;

            case LUA_TBOOLEAN:  /* booleans */
              message += lua_toboolean(state, i) ? "true" : "false";
              break;

            case LUA_TNUMBER:  /* numbers */
              message += std::to_string(lua_tonumber(state, i));
              break;

            default:  /* other values */
              message += lua_typename(state, t);
              break;
          }
          message += "\n";  /* put a separator */
        }

        ScriptFuncs::ReportMessage(state, message.c_str());
      }

      ScriptInternal::PushScriptValue(state, interface->m_DebugStubReturnValues[Index]);
      return 1;
    };

    ScriptInterfaceInitFunc<Index + 1>::Register();
  }

};

template <>
struct ScriptInterfaceInitFunc<kScriptInterfaceMaxFunctions>
{
  static void Register()
  {

  }
};

ScriptInterface::ScriptInterface(NotNullPtr<ScriptState> script_state) :
  m_ScriptState(script_state),
  m_NextFuncId(0),
  m_NextDebugFuncId(0)
{
  if(g_ScriptInterfaceCallbackFuncs[0] == nullptr)
  {
    ScriptInterfaceInitFunc<0>::Register();
  }

  m_Object = std::make_shared<ScriptObject>(script_state->CreateScriptObject());
  ScriptInternal::WriteTablePointer("ptr", *m_Object, this, script_state);
}

ScriptInterface::~ScriptInterface()
{
  ScriptInternal::WriteTablePointer("ptr", *m_Object, nullptr, m_ScriptState);
}

const std::shared_ptr<ScriptObject> & ScriptInterface::GetObject() const
{
  return m_Object;
}

void ScriptInterface::AddVariable(czstr name, const ScriptValue & value)
{
  m_Object->WriteValue(name, value);
}

void ScriptInterface::AddDebugStubFunction(czstr name, ScriptValue && return_value, bool debug_args)
{
  ScriptInternal::WriteTableFunction(name, *m_Object, g_ScriptDebugStubFuncs[m_NextDebugFuncId], m_ScriptState);
  m_DebugStubReturnValues.emplace_back(std::move(return_value));
  m_DebugStubFunctionNames.emplace_back(name);
  m_DebugStubFunctionOutput.emplace_back(debug_args);
  m_NextDebugFuncId++;

  ASSERT(m_NextDebugFuncId == (int)m_DebugStubReturnValues.size(), "Stub function failed sanity check");
}

void ScriptInterface::RegisterFunc(czstr name, int index)
{
  ScriptInternal::WriteTableFunction(name, *m_Object, g_ScriptInterfaceCallbackFuncs[index], m_ScriptState);
}
