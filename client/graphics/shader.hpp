// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#ifndef __APPLE__
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#endif

#include <string>
#include <map>
#include <list>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>

#include "filesystem.hpp"
#include "multiton.hpp"

namespace tec {
	class Shader;
	typedef Multiton<std::string, std::shared_ptr<Shader>> ShaderMap;

	class Shader final {
	public:
		enum ShaderType : GLenum {
			VERTEX = GL_VERTEX_SHADER,
			FRAGMENT = GL_FRAGMENT_SHADER,
			GEOMETRY = GL_GEOMETRY_SHADER,
		};

		Shader();

		~Shader();
		
		/**
		 * \brief Uses the shader program.
		 * \return void
		 */
		void Use();

		/**
		 * \brief Unuses the shader program.
		 * \return void
		 */
		void UnUse();

		/**
		 * \brief Activates the specified texture unit and bind the specified texture to it.
		 * \param const GLuint unit The texture unit to activate.
		 * \param const GLuint texture_name The GL name of the texture to bind.
		 * \return void
		 */
		static void ActivateTextureUnit(const GLuint unit, const GLuint texture_name);

		/**
		 * \brief Deactivates the specified texture unit by binding texture 0 to it.
		 * \param const GLuint unit The texture unit to deactivate.
		 * \return void
		 */
		static void DeactivateTextureUnit(const GLuint unit);

		/**
		 * \brief Returns the location of the specified uniform.
		 * \param const std::string name The name of the uniform to find the location of.
		 * \return GLint The location of the requested uniform.
		 */
		GLint GetUniformLocation(const std::string name);

		/**
		 * \brief Returns the location of the specified attribute.
		 * \param const std::string name The name of the attribute to find the location of.
		 * \return GLint The location of the requested attribute.
		 */
		GLint GetAttributeLocation(const std::string name);

		/**
		 * \brief Creates a Shader from files on disk and stores it in ShaderMap under name.
		 *
		 * Filenames should be something like:
		 * \code{.cpp}:
		 * auto shader_files = std::list < std::pair<Shader::ShaderType, std::string> > {
		 *	std::make_pair(Shader::VERTEX, FilePath("basic.vert")), std::make_pair(Shader::FRAGMENT, FilePath("./basic.frag")),
		 * };
		 * \return is a weak_ptr to the created Shader.
		 */
		static std::shared_ptr<Shader> CreateFromFile(const std::string name, std::list<std::pair<Shader::ShaderType, tec::FilePath>> filenames);
	
		/**
		 * \brief Creates a Shader the provide source code stores it in ShaderMap under name.
		 *
		 * Source_code should be something like:
		 * \code{.cpp}:
		 * auto shader_files = std::list < std::pair<Shader::ShaderType, std::string> > {
		 *   std::make_pair(Shader::VERTEX, "shader source code\nversion 330\n{}"), std::make_pair(Shader::FRAGMENT, "shader source code\nversion 330\n{}"),
		 * };
		 * \return is a weak_ptr to the created Shader.
		 */
		static std::shared_ptr<Shader> CreateFromString(const std::string name, std::list<std::pair<Shader::ShaderType, std::string>> source_code);

		/**
		 * \brief Loads the specified ShaderType from file filename.
		 * \param const ShaderType type The type of shader that is being loaded (VERTEX, FRAGMENT, GEOMETRY).
		 * \param const FilePath filename The filename of the source file to load (relative to assets folder)
		 * \return void
		 */
		void LoadFromFile(const ShaderType type, const tec::FilePath& filename);

		/**
		 * \brief Loads the specified ShaderType from the source string provided..
		 * \param const ShaderType type The type of shader that is being loaded (VERTEX, FRAGMENT, GEOMETRY).
		 * \param const std::string source The source string to load from.
		 * \return void
		 */
		void LoadFromString(const ShaderType type, const std::string& source) {
			LoadFromString(type, source, "");
		}

		/**
		 * \brief Builds the shader program after all shaders have been loaded.
		 * This is called automatically from the factory methods.
		 * \return void
		 */
		void Build();

		/**
		 * \brief Deletes the shader program.
		 * \return void
		 */
		void DeleteProgram();
	private:

		/**
		* \brief Loads the specified ShaderType from the source string provided..
		* \param const ShaderType type The type of shader that is being loaded (VERTEX, FRAGMENT, GEOMETRY).
		* \param const std::string source The source string to load from.
		* \param const std::string filename The filename from were the source comes (only for debug purposes)
		* \return void
		*/
		void LoadFromString(const ShaderType type, const std::string& source, const std::string& filename);

		GLuint program;
		std::vector<GLuint> shaders;
		std::map<std::string, GLint> attributes;
		std::map<std::string, GLint> uniforms;
	};
}
