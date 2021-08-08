#include "shader.hpp"

#include <iterator>

#include <spdlog/spdlog.h>

namespace tec {
Shader::~Shader() { DeleteProgram(); }

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

void Shader::LoadFromFile(const gfx::ShaderType type, const tec::FilePath& fname) {
	auto _log = spdlog::get("console_log");
	if (!fname.isValidPath()) {
		_log->error("[Shader] Error loading shader: {} Invalid path: {}", fname.FileName(), fname.toString());
		return;
	}
	if (!fname.FileExists()) {
		_log->error("[Shader] Error loading shader: {} File don't exists. Check assets folder", fname.FileName());
		return;
	}
	std::ifstream fp(fname.GetNativePath(), std::ios_base::in);
	if (!fp.is_open()) {
		_log->error("[Shader] Error loading shader: {} File don't exists. Check open file.", fname.FileName());
		return;
	}
	std::string buffer(std::istreambuf_iterator<char>(fp), (std::istreambuf_iterator<char>()));
	LoadFromString(type, buffer, fname.FileName());
}

void Shader::LoadFromString(const gfx::ShaderType type, const std::string& source, const std::string& filename) {
	auto _log = spdlog::get("console_log");
	glGetError();
	GLuint shader = glCreateShader(type);
	if (auto error = glGetError() != GL_NO_ERROR) {
		_log->error("[Shader] Error creating shader : {} : {}", filename, error);
		return;
	}

	GLint length = static_cast<GLint>(source.length());
	const GLchar* str = source.data();
	glShaderSource(shader, 1, &str, &length);
	if (auto error = glGetError() != GL_NO_ERROR) {
		_log->error("[Shader] Error loading shader source : {} : {}", filename, error);
		return;
	}

	glCompileShader(shader);
	if (auto error = glGetError() != GL_NO_ERROR) {
		_log->error("[Shader] Error compiling shader : {} : {}", filename, error);
		return;
	}

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
		GLchar* info_log = new GLchar[log_length];
		glGetShaderInfoLog(shader, log_length, NULL, info_log);
		_log->error("[Shader] Error compiling shader : {} : {}", filename, info_log);
		delete[] info_log;
	}

	if (auto error = glGetError() != GL_NO_ERROR) {
		_log->error("[Shader] Error compiling shader : {} : {}", filename, error);
		return;
	}
	this->shaders.push_back(shader);
}

bool Shader::Build() {
	this->program = glCreateProgram();

	for (auto shader : this->shaders) {
		glAttachShader(this->program, shader);
	}

	glLinkProgram(this->program);

	GLint is_linked = 0;
	glGetProgramiv(this->program, GL_LINK_STATUS, &is_linked);
	if (is_linked == GL_FALSE) {
		GLint max_length = 0;
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &max_length);

		if (max_length > 0) {
			std::vector<GLchar> info_log(max_length);
			glGetProgramInfoLog(this->program, max_length, &max_length, &info_log[0]);
			std::string str(info_log.data());
			spdlog::get("console_log")->error("[Shader] Error linking : {}", str);
		}

		DeleteProgram();

		return false;
	}

	for (auto shader : this->shaders) {
		glDetachShader(this->program, shader);
	}
	return true;
}

void Shader::Use() { glUseProgram(this->program); }

void Shader::UnUse() { glUseProgram(0); }

void Shader::ActivateTextureUnit(const GLuint unit, const GLuint texture_name) {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture_name);
}

void Shader::DeactivateTextureUnit(const GLuint unit) {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLint Shader::GetUniformLocation(const std::string name) {
	auto uniform_itr = this->uniforms.find(name);
	if (uniform_itr != this->uniforms.end()) {
		return uniform_itr->second;
	}
	else {
		GLint uniform_id = glGetUniformLocation(this->program, name.c_str());
		if (uniform_id) {
			this->uniforms[name] = uniform_id;
		}
		return uniform_id;
	}
}

GLint Shader::GetAttributeLocation(const std::string name) {
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
}

std::shared_ptr<Shader>
Shader::CreateFromFile(const std::string name, std::list<std::pair<gfx::ShaderType, FilePath>> filenames) {
	auto s = std::make_shared<Shader>();
	for (auto pair : filenames) {
		s->LoadFromFile(pair.first, pair.second);
	}
	if (s->Build()) {
		ShaderMap::Set(name, s);
		return s;
	}
	return nullptr;
}

std::shared_ptr<Shader>
Shader::CreateFromString(const std::string name, std::list<std::pair<gfx::ShaderType, std::string>> source_code) {
	auto s = std::make_shared<Shader>();
	for (auto pair : source_code) {
		s->LoadFromString(pair.first, pair.second);
	}
	if (s->Build()) {
		ShaderMap::Set(name, s);
		return s;
	}
	return nullptr;
}

void Shader::LoadFromProto(const gfx::ShaderSource& source, gfx::ShaderType type) {
	if (source.has_type()) {
		type = source.type();
	}
	switch (source.source_case()) {
	case gfx::ShaderSource::kFile: LoadFromFile(type, FilePath::GetAssetPath("shaders") / source.file()); break;
	case gfx::ShaderSource::kAbsFile: LoadFromFile(type, FilePath(source.abs_file())); break;
	case gfx::ShaderSource::kRaw: LoadFromString(type, source.raw(), "proto-" + std::to_string(type)); break;
	case gfx::ShaderSource::kByName:
		spdlog::get("console_log")->error("[Shader] LoadFromProto by_name not implemented");
		break;
	case gfx::ShaderSource::SOURCE_NOT_SET:
	default: break;
	}
}

std::shared_ptr<Shader> Shader::CreateFromDef(const gfx::ShaderDef& shader_def) {
	auto s = std::make_shared<Shader>();
	for (auto& source : shader_def.files()) {
		s->LoadFromProto(source);
	}
	if (shader_def.has_vertex()) {
		s->LoadFromProto(shader_def.vertex(), gfx::VERTEX);
	}
	if (shader_def.has_fragment()) {
		s->LoadFromProto(shader_def.fragment(), gfx::FRAGMENT);
	}
	if (shader_def.has_geometry()) {
		s->LoadFromProto(shader_def.geometry(), gfx::GEOMETRY);
	}
	if (shader_def.has_tess_control()) {
		s->LoadFromProto(shader_def.tess_control(), gfx::TESS_CONTROL);
	}
	if (shader_def.has_tess_eval()) {
		s->LoadFromProto(shader_def.tess_eval(), gfx::TESS_EVAL);
	}
	if (shader_def.has_compute()) {
		s->LoadFromProto(shader_def.compute(), gfx::COMPUTE);
	}
	if (s->Build()) {
		ShaderMap::Set(shader_def.name(), s);
		return s;
	}
	return nullptr;
}

} // namespace tec
