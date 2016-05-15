// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <memory>
#include <vector>

#include "multiton.hpp"

#ifndef __APPLE__
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#endif

namespace tec {
	class TextureObject;

	class Material;
	typedef Multiton<std::string, std::shared_ptr<Material>> MaterialMap;

	class Material {
	public:
		Material();

		/**
		 * \brief Factory method that creates a Material and stores it in the
		 * MaterialMap under name.
		 * \param const std::string name The name to store the PixelBuffer under.
		 * \param std::shared_ptr<Shader> shader The shader this material uses.
		 * \return std::shared_ptr<Material> The created Material.
		 */
		static std::shared_ptr<Material> Create(const std::string name);
		
		/**
		 * \brief Adds a TextureObject to this material.
		 * \param std::shared_ptr<TextureObject> tex The texture to add.
		 * \return void
		 */
		void AddTexture(std::shared_ptr<TextureObject> tex);
		
		/**
		 * \brief Gets a TextureObject from this material.
		 * \param size_t index The index of the texture to grab (0-based).
		 * \return std::shared_ptr<TextureObject> The request texture, or nullptr if index is invalid.
		 */
		std::shared_ptr<TextureObject> GetTexutre(std::size_t index);

		/**
		 * \brief Removes a TextureObject from this material.
		 * \param std::shared_ptr<TextureObject> tex The texture to remove.
		 * \return void
		 */
		void RemoveTexture(std::shared_ptr<TextureObject> tex);
	
		/**
		 * \brief Sets the PolygonMode used when this material is in use.
		 * \param const GLenum mode The PolygonMode (GL_POINT, GL_LINE, or GL_FILL).
		 * \return void
		 */
		void SetPolygonMode(const GLenum mode);

		/**
		 * \brief Gets the PolygonMode of this material.
		 * \return const GLenum The PolygonMode of this material.
		 * \return void
		 */
		const GLenum GetPolygonMode();

		/**
		 * \brief Sets the DrawElements type used when this material is in use.
		 * \param const GLenum mode The DrawElements (GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP,
		 * GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, or GL_PATCHES).
		 * \return void
		 */
		void SetDrawElementsMode(const GLenum mode);

		/**
		 * \brief Gets the DrawElements mode of this material.
		 * \return const GLenum The DrawElements mode of this material.
		 * \return void
		 */
		const GLenum GetDrawElementsMode() const;

		/**
		 * \brief Activates all the textures used by this material.
		 * \return void
		 */
		void Activate();

		/**
		 * \brief Deactivates all the textures used by this material.
		 * \return void
		 */
		void Deactivate();
	private:
		GLenum polygon_mode;
		GLenum draw_elements_mode;
		std::vector<std::shared_ptr<TextureObject>> textures;
	};
}
