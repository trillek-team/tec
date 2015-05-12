#include "shader.hpp"

#include <iterator>

namespace vv {
	Shader::Shader() : program(0) { }

	Shader::~Shader() {
		DeleteProgram();
	}

	void Shader::DeleteProgram() {
		if (this->program != 0) {
			glDeleteProgram(this->program);
		}
		this->program = 0;
		for (auto s : this->shaders) {
			glDeleteShader(s);
		}
		this->shaders.clear();
	}

	void Shader::LoadFromFile(const ShaderType type, const std::string fname) {
		std::ifstream fp;
		fp.open(fname, std::ios_base::in);
		if (fp.is_open()) {
			std::string buffer(std::istreambuf_iterator<char>(fp), (std::istreambuf_iterator<char>()));
			LoadFromString(type, buffer);
		}
	}

	void Shader::LoadFromString(const ShaderType type, const std::string source) {
		glGetError();
		GLuint shader = glCreateShader(type);
		if (glGetError()) {
			return;
		}

		GLint length = source.length();
		const GLchar *str = source.data();
		glShaderSource(shader, 1, &str, &length);
		if (glGetError()) {
			return;
		}

		glCompileShader(shader);
		if (glGetError()) {
			return;
		}
		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE) {
			GLint log_length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
			GLchar *info_log = new GLchar[log_length];
			glGetShaderInfoLog(shader, log_length, NULL, info_log);
			std::cout << "Error compiling shader: " << info_log;
			delete[] info_log;
		}
		if (glGetError()) {
			return;
		}
		this->shaders.push_back(shader);
	}

	void Shader::Build() {
		this->program = glCreateProgram();

		for (auto shader : this->shaders) {
			glAttachShader(this->program, shader);
		}

		glLinkProgram(this->program);

		GLint is_linked = 0;
		glGetProgramiv(this->program, GL_LINK_STATUS, (int *)&is_linked);
		if (is_linked == GL_FALSE) {
			GLint max_length = 0;
			glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &max_length);

			std::vector<GLchar> info_log(max_length);
			glGetProgramInfoLog(this->program, max_length, &max_length, &info_log[0]);
			std::copy(info_log.begin(), info_log.end(), std::ostream_iterator<GLchar>(std::cout, ""));

			DeleteProgram();

			return;
		}

		for (auto shader : this->shaders) {
			glDetachShader(this->program, shader);
		}
	}

	void Shader::Use() {
		glUseProgram(this->program);
	}

	void Shader::UnUse() {
		glUseProgram(0);
	}

	void Shader::ActivateTextureUnit(const GLuint unit, const GLuint name) {

	}

	void Shader::DeactivateTextureUnit(const GLuint unit) {

	}

	GLint Shader::GetUniform(const std::string name) {
		if (this->uniforms.find(name) != this->uniforms.end()) {
			return this->uniforms.at(name);
		}
		else {
			GLint uniform_id = glGetUniformLocation(this->program, name.c_str());
			if (uniform_id) {
				this->uniforms[name] = uniform_id;
			}
			return uniform_id;
		}
		return 0;
	}

	GLint Shader::GetAttribute(const std::string name) {
		if (this->attributes.find(name) != this->attributes.end()) {
			return this->attributes.at(name);
		}
		else {
			GLint attribute_id = glGetAttribLocation(this->program, name.c_str());
			if (attribute_id) {
				this->attributes[name] = attribute_id;
			}
			return attribute_id;
		}
		return 0;
	}

	std::weak_ptr<Shader> Shader::CreateFromFile(const std::string name,
		std::list<std::pair<Shader::ShaderType, std::string>> filenames) {
		auto s = std::make_shared<Shader>();
		for (auto pair : filenames) {
			s->LoadFromFile(pair.first, pair.second);
		}
		s->Build();
		ShaderMap::Set(name, s);
		return s;
	}

	std::weak_ptr<Shader> Shader::CreateFromString(const std::string name,
		std::list<std::pair<Shader::ShaderType, std::string>> source_code) {
		auto s = std::make_shared<Shader>();
		for (auto pair : source_code) {
			s->LoadFromString(pair.first, pair.second);
		}
		s->Build();
		ShaderMap::Set(name, s);
		return s;
	}
}
