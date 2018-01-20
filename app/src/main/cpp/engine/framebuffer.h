#pragma once

#include "../pch.h"
#include <mutex>

namespace Engine {

	class Framebuffer
	{
	private:

		GLsizei m_width;
		GLsizei m_height;
		GLenum	m_format;

		GLuint m_handle;
		GLuint m_texture;
		GLuint m_depth;

		static void createTexture(GLuint texture, GLsizei width, GLsizei height, GLenum format)
		{
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,format, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}

		static void createDepthTexture(GLuint texture, GLsizei width, GLsizei height)
		{
			glBindRenderbuffer(GL_RENDERBUFFER, texture);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		}

	public:

		Framebuffer()
			: m_width(0)
			, m_height(0)
			, m_handle(0)
			, m_texture(0)
			, m_depth(0)
		{
		}

		virtual ~Framebuffer()
		{
			release();
		}

		void release()
		{
			if(m_texture > 0)
			{
				glDeleteTextures(1, &m_texture);
				m_texture = 0;
			}

			if(m_depth > 0)
			{
				glDeleteTextures(1, &m_depth);
				m_depth = 0;
			}

			if(m_handle > 0)
			{
				glDeleteFramebuffers(1, &m_handle);
				m_handle = 0;
			}
		}

		bool init(int width, int height, bool alpha, bool depth);

		void set() const
		{
			if(m_handle > 0)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
				glViewport(0, 0, m_width, m_height);
			}
		}

		void clear()
		{
			if(m_depth > 0)
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			else
				glClear(GL_COLOR_BUFFER_BIT);
		}

		void bind() const
		{
			if(m_texture > 0)
				glBindTexture(GL_TEXTURE_2D, m_texture);
		}

		GLsizei getWidth() const { return m_width; }
		GLsizei getHeight() const { return m_height; }
	};
};