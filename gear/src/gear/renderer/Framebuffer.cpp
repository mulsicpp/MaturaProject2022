#include "Framebuffer.h"
#include <glad/glad.h>

void gear::Framebuffer::create(int width, int height)
{
  glCreateTextures(GL_TEXTURE_2D, 1, &m_Texture);
  glBindTexture(GL_TEXTURE_2D, m_Texture);

  m_Width = width;
  m_Height = height;

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glCreateFramebuffers(1, &m_Framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_Texture, 0);

  glCreateRenderbuffers(1, &m_Depthbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_Depthbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_Depthbuffer);

  unsigned int draw_Buffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, draw_Buffers);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    gear::error("Framebuffer couldn't be completed");
}

void gear::Framebuffer::destroy(void)
{
  glDeleteTextures(1, &m_Texture);
  glDeleteRenderbuffers(1, &m_Depthbuffer);
  glDeleteFramebuffers(1, &m_Framebuffer);
}

void gear::Framebuffer::set_Size(int width, int height)
{
  m_Width = width;
  m_Height = height;

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
}

void gear::Framebuffer::bind(void)
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
  glViewport(0, 0, m_Width, m_Height);
}