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

#include "multiton.hpp"

namespace vv {
	class Shader;

	typedef Multiton<std::string, std::shared_ptr<Shader>> ShaderMap;

	class Shader {
	public:
		enum ShaderType : GLenum {
			VERTEX = GL_VERTEX_SHADER,
			FRAGMENT = GL_FRAGMENT_SHADER,
			GEOMETRY = GL_GEOMETRY_SHADER,
		};

		Shader();

		~Shader();

		void Use();

		void UnUse();

		void ActivateTextureUnit(const GLuint unit, const GLuint name);

		static void DeactivateTextureUnit(const GLuint unit);

		GLint GetUniform(const std::string name);

		GLint GetAttribute(const std::string name);

		// Creates a Shader from files on disk and stores it in ShaderMap under name.
		// filenames should be something like:
		// auto shader_files = std::list < std::pair<vv::Shader::ShaderType, std::string> > {
		//	std::make_pair(vv::Shader::VERTEX, "basic.vert"), std::make_pair(vv::Shader::FRAGMENT, "basic.frag"),
		// };
		// return is a weak_ptr to the created Shader.
		static std::weak_ptr<Shader> CreateFromFile(const std::string name, std::list<std::pair<Shader::ShaderType, std::string>> filenames);

		// Creates a Shader from the provide source code stores it in ShaderMap under name.
		// source_code should be something like:
		// auto shader_files = std::list < std::pair<vv::Shader::ShaderType, std::string> > {
		//	std::make_pair(vv::Shader::VERTEX, "shader source code\nversion 330\n{}"), std::make_pair(vv::Shader::FRAGMENT, "shader source code\nversion 330\n{}"),
		// };
		// return is a weak_ptr to the created Shader.
		static std::weak_ptr<Shader> CreateFromString(const std::string name, std::list<std::pair<Shader::ShaderType, std::string>> source_code);
	private:
		void DeleteProgram();

		void LoadFromFile(const ShaderType type, const std::string fname);

		void LoadFromString(const ShaderType type, const std::string source);

		void Build();

		GLuint program;
		std::vector<GLuint> shaders;
		std::map<std::string, GLint> attributes;
		std::map<std::string, GLint> uniforms;
	};
}
