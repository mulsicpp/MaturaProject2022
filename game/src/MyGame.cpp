#include "MyGame.h"

#include <gear/data/FileStream.h>
#include <gear/resource/ResourceManager.h>

#include <gear/event/event_Types/KeyEvent.h>
#include <gear/event/EventComponent.h>
#include <gear/event/Input.h>

void MyGame::on_Startup(void)
{
  m_Window->set_Size(1280, 720);
  m_Window->set_Title("AHHHHHHH!");
  m_Window->set_Visible(true);
  //  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG("Opened application");

  gear::EventComponent<gear::KeyEvent> e = {[](gear::KeyEvent e){}, false};
}

void MyGame::per_Frame(void)
{
  if (m_Window->should_Close())
    this->close(0);
  gear::Input::dispatch_Events(nullptr);
  m_Window->swap_Buffers();
  m_Window->poll_Events();
}

void MyGame::on_Shutdown(void)
{
  GEAR_DEBUG_LOG("unloaded resources: %i", gear::ResourceManager::unload());
  GEAR_DEBUG_LOG("Closed application");
}