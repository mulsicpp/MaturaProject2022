#include "MyGame.h"

#include <gear/data/FileStream.h>
#include <gear/resource/ResourceManager.h>
#include <gear/renderer/Renderer.h>

void MyGame::on_Startup(void)
{
  m_Window->set_Size(1280, 720);
  m_Window->set_Title("Game Window!");
  m_Window->set_Visible(true);

  gear::Renderer::create(640, 360);
  gear::Renderer::set_V_Sync(true);
  gear::Renderer::setup_Test_Frame();
  
  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG("Opened application");
}

void MyGame::per_Frame(void)
{
  if (m_Window->should_Close())
    this->close(0);
  
  gear::Renderer::start_New_Frame();
  gear::Renderer::show_Frame();

  m_Window->poll_Events();
}

void MyGame::on_Shutdown(void)
{
  GEAR_DEBUG_LOG("unloaded resources: %i", gear::ResourceManager::unload());
  GEAR_DEBUG_LOG("Closed application");
}