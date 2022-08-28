#pragma once

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "filesystem.hpp"
#include "multiton.hpp"
#include <graphics.pb.h>

namespace tec {
class Shader;
typedef Multiton<std::string, std::shared_ptr<Shader>> ShaderMap;
typedef Multiton<std::string, std::string> ShaderIncludes;

class Shader final {
public:
	Shader() = default;

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
	*	std::make_pair(Shader::VERTEX, Path("basic.vert")), std::make_pair(Shader::FRAGMENT,
	*Path("./basic.frag")),
	* };
	* \return is a shared_ptr to the created Shader.
	*/
	static std::shared_ptr<Shader>
	CreateFromFile(const std::string name, std::list<std::pair<gfx::ShaderType, tec::Path>> filenames);

	/**
	* \brief Creates a Shader the provide source code stores it in ShaderMap under name.
	*
	* Source_code should be something like:
	* \code{.cpp}:
	* auto shader_files = std::list < std::pair<Shader::ShaderType, std::string> > {
	*   std::make_pair(Shader::VERTEX, "shader source code\nversion 330\n{}"),
	* std::make_pair(Shader::FRAGMENT, "shader source code\nversion 330\n{}"),
	* };
	* \return is a shared_ptr to the created Shader.
	*/
	static std::shared_ptr<Shader>
	CreateFromString(const std::string name, std::list<std::pair<gfx::ShaderType, std::string>> source_code);

	/**
	* \brief Creates a Shader using parameters from a tec::gfx::ShaderDef protocol object.
	*
	* a ShaderDef should contain a name and assets relative filenames.
	* \return is a shared_ptr to the created Shader.
	*/
	static std::shared_ptr<Shader> CreateFromDef(const gfx::ShaderDef& shader_def);

	/**
	* \brief Adds an item to the shader includes from the tec::gfx::ShaderInclude protocol.
	*
	* a ShaderDef which should contain a name and assets relative filenames.
	*/
	static void IncludeFromDef(const gfx::ShaderInclude& shader_inc);

	/**
	* \brief Loads the specified ShaderType from file filename.
	* \param const ShaderType type The type of shader that is being loaded i.e. VERTEX, FRAGMENT, etc.
	* \param const Path filename The filename of the source file to load
	* (relative to assets folder)
	* \return void
	*/
	void LoadFromFile(const gfx::ShaderType type, const tec::Path& filename);

	/**
	* \brief Loads the specified ShaderType from the source string provided..
	* \param const ShaderType type The type of shader that is being loaded i.e. VERTEX, FRAGMENT, etc.
	* \param const std::string source The source string to load from.
	* \return void
	*/
	void LoadFromString(const gfx::ShaderType type, const std::string& source) { LoadFromString(type, source, ""); }

	/**
	* \brief Loads the specified ShaderType from the source string provided..
	* \param source The ShaderSource object to load from.
	* \param ShaderType optional type of the shader being loaded if object does not specify it, i.e. VERTEX, FRAGMENT, etc.
	* \return void
	*/
	void LoadFromProto(const gfx::ShaderSource& source, gfx::ShaderType type = gfx::ShaderType::VERTEX);

	/**
	* \brief Builds the shader program after all shaders have been loaded.
	* This is called automatically from the factory methods.
	* \param name The name of the shader, printed out during link errors
	* \return true if the shader built successfully
	*/
	bool Build(const std::string& name);

	/**
	* \brief Deletes the shader program.
	* \return void
	*/
	void DeleteProgram();

private:
	/**
	* \brief Loads the specified ShaderType from the source string provided..
	* \param const ShaderType type The type of shader that is being loaded (VERTEX, FRAGMENT,
	* GEOMETRY).
	* \param const std::string source The source string to load from.
	* \param const std::string filename The filename from were the source comes (only for
	* debug purposes)
	* \return void
	*/
	void LoadFromString(const gfx::ShaderType type, const std::string& source, const std::string& filename);

	GLuint program{0};
	std::vector<GLuint> shaders;
	std::unordered_map<std::string, GLint> attributes;
	std::unordered_map<std::string, GLint> uniforms;
	bool run_initial{true};
	std::vector<std::pair<GLint, GLint>> initial_1i;
};
} // namespace tec
