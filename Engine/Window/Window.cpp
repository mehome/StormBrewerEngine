
#include "Engine/EngineCommon.h"
#include "Engine/Window/Window.h"
#include "Engine/Window/WindowManager.h"
#include "Engine/Input/TextInputContext.h"

Window::Window() :
  m_WindowId(0)
{

}

Window::Window(uint32_t window_id) :
  m_WindowId(window_id)
{

}

Window::Window(const Window & rhs) :
  m_WindowId(rhs.m_WindowId)
{
  AddRef();
}

Window::Window(Window && rhs) :
  m_WindowId(rhs.m_WindowId)
{
  rhs.m_WindowId = 0;
}

Window::~Window()
{
  DecRef();
}

Window & Window::operator = (const Window & rhs)
{
  DecRef();

  m_WindowId = rhs.m_WindowId;
  AddRef();
  return *this;
}

Window & Window::operator = (Window && rhs)
{
  DecRef();
  m_WindowId = rhs.m_WindowId;
  rhs.m_WindowId = 0;
  return *this;
}


void Window::MakeCurrent() const
{
  if (m_WindowId == 0)
  {
    return;
  }

  g_WindowManager.MakeCurrent(m_WindowId);
}

void Window::Swap() const
{
  if (m_WindowId == 0)
  {
    return;
  }

  g_WindowManager.Swap(m_WindowId);
}

NullOptPtr<InputState> Window::GetInputState() const
{
  if (m_WindowId == 0)
  {
    return nullptr;
  }

  return g_WindowManager.GetInputState(m_WindowId);
}

std::shared_ptr<TextInputContext> Window::CreateTextInputContext()
{
  return std::shared_ptr<TextInputContext>(new TextInputContext(m_WindowId));
}

void Window::AddRef()
{
  if (m_WindowId == 0)
  {
    return;
  }

  g_WindowManager.AddWindowRef(m_WindowId);
}

void Window::DecRef()
{
  if (m_WindowId == 0)
  {
    return;
  }

  g_WindowManager.DecWindowRef(m_WindowId);
}