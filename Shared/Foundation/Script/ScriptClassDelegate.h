
#pragma once

#include "Foundation/Delegate/Delegate.h"

#include "Foundation/Script/ScriptFuncPtr.h"

template <typename ReturnType, typename ... Args>
class ScriptClassDelegate
{
public:

  ScriptClassDelegate() = default;
  ScriptClassDelegate(const ScriptClassDelegate & rhs) = delete;
  ScriptClassDelegate(ScriptClassDelegate && rhs) = delete;
  ~ScriptClassDelegate() = default;

  ScriptClassDelegate & operator = (const ScriptClassDelegate & rhs) = delete;
  ScriptClassDelegate & operator = (ScriptClassDelegate && rhs) = delete;

  template <typename Callable, typename std::enable_if_t<
          !std::is_same<std::decay_t<Callable>, Delegate<ReturnType, Args...>>::value &&
          std::is_class<std::decay_t<Callable>>::value> * Enable = nullptr>
  ScriptClassDelegate & operator = (Callable && callable)
  {
    m_FuncPtr.Clear();
    m_Delegate = std::forward<Callable>(callable);
  }

  ReturnType Call(Args ... args)
  {
    return m_Delegate.Call(std::forward<Args>(args)...);
  }

  ReturnType operator()(Args ... args)
  {
    return m_Delegate.Call(std::forward<Args>(args)...);
  }

  bool IsValid() const
  {
    return m_Delegate.IsValid();
  }

private:

  template <typename T>
  friend struct ScriptClassPushMember;

  template <typename T>
  friend struct ScriptClassAssignMember;

  template <typename T>
  friend class ScriptClass;

  friend class ScriptClassBase;

  ScriptFuncPtr m_FuncPtr;
  Delegate<ReturnType, Args...> m_Delegate;
};
