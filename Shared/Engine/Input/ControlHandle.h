#pragma once

#include "Foundation/Handle/Handle.h"

#include "Engine/Input/ControlId.h"

class InputState;

class ENGINE_EXPORT ControlHandle
{
protected:
  ControlHandle();
  ControlHandle(NotNullPtr<InputState> input_state, Handle handle, ControlId control_id);

  void * GetControlHandle() const;

protected:
  friend class KeyboardState;
  friend class MouseState;

  InputState * m_InputState;
  Handle m_Control;
  ControlId m_ControlId;
};

