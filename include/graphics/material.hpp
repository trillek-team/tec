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
	class Shader;
	class TextureObject;

	class Material;
	typedef Multiton<std::string, std::shared_ptr<Material>> MaterialMap;

	class Material {
	public:
		Material(const std::weak_ptr<Shader> shader) : shader(shader), polygon_mode(GL_FILL) { }

		/**
		 * \brief Sets the shader used by this material.
		 * \param std::shared_ptr<Shader> s The shader to use.
		 * \return void
		 */
		void SetShader(std::shared_ptr<Shader> s);

		/**
		 * \brief Sets the shader used by this material, looking it up in the ShaderMap.
		 * \param const std::string name The name of the shader to look up.
		 * \return void
		 */
		void SetShader(const std::string name);

		/**
		 * \brief Gets the shader used by this material.
		 * \return std::shared_ptr<Shader> The shader used by this material.
		 */
		std::shared_ptr<Shader> GetShader();

		/**
		 * \brief Factory method that creates a Material and stores it in the
		 * MaterialMap under name.
		 * \param const std::string name The name to store the PixelBuffer under.
		 * \param std::shared_ptr<Shader> shader The shader this material uses.
		 * \return std::shared_ptr<Material> The created Material.
		 */
		static std::shared_ptr<Material> Create(const std::string name, std::shared_ptr<Shader> shader);
		
		/**
		 * \brief Adds a TextureObject to this material.
		 * \param std::shared_ptr<TextureObject> tex The texture to add.
		 * \return void
		 */
		void AddTexture(std::shared_ptr<TextureObject> tex);

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
		std::shared_ptr<Shader> shader;
		std::vector<std::shared_ptr<TextureObject>> textures;
	};
}
