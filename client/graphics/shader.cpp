#include "shader.hpp"

#include <iterator>
#include <sstream>
#include <string_view>

#include <spdlog/spdlog.h>

#include "gbuffer.hpp"
#include "proto-load.hpp"
#include "render-system.hpp"

namespace tec {

static const std::map<std::string_view, GLint> engine_constants{
		{"gCompositeMap", (GLint)GBuffer::TEXTURE_TYPE::TEXTURE_TYPE_LAST},
		{"gPositionMap", (GLint)GBuffer::TEXTURE_TYPE::POSITION},
		{"gNormalMap", (GLint)GBuffer::TEXTURE_TYPE::NORMAL},
		{"gColorMap", (GLint)GBuffer::TEXTURE_TYPE::DIFFUSE},
		{"gEmissionMap", (GLint)GBuffer::TEXTURE_TYPE::EMISSION},
		{"gDepthMap", (GLint)GBuffer::DEPTH_TYPE::DEPTH},
		{"mColorMap", 0},
		{"mSPMap", 1},
		{"mNormalMap", 2},
		{"mMap0", 0},
		{"mMap1", 1},
		{"mMap2", 2},
		{"mMap3", 3},
};

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

void Shader::LoadFromFile(const gfx::ShaderType type, const tec::Path& fname) {
	auto _log = spdlog::get("console_log");
	if (!fname) {
		_log->error("[Shader] Load Error: Invalid path: {}", fname.toString());
		return;
	}
	if (!fname.FileExists()) {
		_log->error("[Shader] Load Error: File does not exist, Check assets folder: {}", fname.toString());
		return;
	}
	auto fp = fname.OpenStream(); // TODO GetFileContent API?
	if (!fp->is_open()) {
		_log->error("[Shader] Load Error: Can not open file: {}", fname);
		return;
	}
	std::string buffer(std::istreambuf_iterator<char>(*fp), (std::istreambuf_iterator<char>()));
	LoadFromString(type, buffer, fname.FileName());
}

void Shader::LoadFromString(const gfx::ShaderType type, const std::string& raw_source, const std::string& filename) {
	auto _log = spdlog::get("console_log");
	glGetError();
	GLuint shader = glCreateShader(type);
	if (auto error = glGetError() != GL_NO_ERROR) {
		_log->error("[Shader] Error creating shader : {} : {}", filename, error);
		return;
	}

	std::stringstream stream;
	std::stringstream in_stream{raw_source};
	size_t line_number = 0;
	using namespace std::string_view_literals;
	constexpr auto npos = std::string::npos;
	auto version_line = "#version "sv;
	auto include_line = "#include "sv;
	size_t next_filenum = 1;
	std::map<size_t, std::string> included_map;
	for (std::string line; std::getline(in_stream, line);) {
		line_number++;
		if (0 == line.compare(0, version_line.size(), version_line)) {
			stream << line << '\n'; // emit the version line, nothing should come before it
			// engine defines go here
			stream << "#define TEC_VERSION 1\n"sv;
			// fixup the line numbers to match the source file
			stream << "#line "sv << line_number + 1 << ' ' << 0 << '\n';
		}
		else if (0 == line.compare(0, include_line.size(), include_line)) {
			// shader #include directives
			// there is an ARB extension for this, which might be worth checking support for
			// it offers a named include tree, with a file-like structure and recursive include
			// TODO this implementation does not have those things, add them or use the ARB extension.
			size_t start;
			size_t end = npos;
			if (npos != (start = line.find_first_of('"'))) {
				start++;
				end = line.find_first_of('"', start);
			}
			else if (npos != (start = line.find_first_of('<'))) {
				start++;
				end = line.find_first_of('>', start);
			}
			std::string include{line.substr(start, end - start)};
			if (!ShaderIncludes::Has(include)) {
				_log->error(
						"[Shader] Error preprocessing source: {}:{} : include not found <{}>",
						filename,
						line_number,
						include);
				return;
			}
			included_map[next_filenum] = include;
			// fixup the line number to match those in the included file
			stream << "#line "sv << 1 << ' ' << next_filenum << '\n';
			std::stringstream include_stream{ShaderIncludes::Get(include)};
			for (std::string incline; std::getline(include_stream, incline);) {
				stream << incline << '\n';
			}
			next_filenum++;
			// fixup the line number to match the original source file
			stream << "#line "sv << line_number + 1 << ' ' << 0 << '\n';
		}
		else {
			stream << line << '\n';
		}
	}
	std::string processed_source = stream.str();
	GLint length = static_cast<GLint>(processed_source.length());
	const GLchar* str = processed_source.data();
	glShaderSource(shader, 1, &str, &length);
	if (auto error = glGetError() != GL_NO_ERROR) {
		_log->error("[Shader] Error loading shader source : {} : {}", filename, error);
		return;
	}

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
		std::string info_log;
		info_log.resize(log_length);
		glGetShaderInfoLog(shader, log_length, NULL, info_log.data());
		_log->error("[Shader] Error compiling shader : {} : {}", filename, info_log);
	}

	if (auto error = glGetError() != GL_NO_ERROR) {
		_log->error("[{}:{}] Unexpected Error: {}", __FILE__, __LINE__, error);
		return;
	}
	this->shaders.push_back(shader);
}

bool Shader::Build(const std::string& name) {
	auto _log = spdlog::get("console_log");
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
			_log->error("[Shader] Error linking {}: {}", name, str);
		}

		DeleteProgram();

		return false;
	}

	GLint active_uniforms;
	GLint active_uniform_max;
	GLint active_attribs;
	GLint active_attrib_max;
	glGetProgramiv(this->program, GL_ACTIVE_UNIFORMS, &active_uniforms);
	glGetProgramiv(this->program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &active_uniform_max);
	glGetProgramiv(this->program, GL_ACTIVE_ATTRIBUTES, &active_attribs);
	glGetProgramiv(this->program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &active_attrib_max);
	std::string max_read;
	max_read.resize(std::max(active_uniform_max, active_attrib_max));

	for (GLint index = 0; index < active_uniforms; index++) {
		GLsizei written;
		GLint uniform_size;
		GLenum sym_type;
		glGetActiveUniform(this->program, index, max_read.size(), &written, &uniform_size, &sym_type, max_read.data());
		std::string uniform_name = max_read.substr(0, max_read.find_first_of('\0'));
		std::string s_type{GLSymbol::Get(sym_type).name};
		GLint uniform_loc = this->GetUniformLocation(uniform_name);
		_log->trace("[Shader] {}/uniform: {}: {} {}: {}", name, uniform_loc, uniform_size, s_type, uniform_name);
		if (sym_type == GL_SAMPLER_2D) {
			auto engconst_itr = engine_constants.find(uniform_name);
			_log->trace("[Shader] ECU lookup: {} found={}", uniform_name, engconst_itr != engine_constants.cend());
			if (engconst_itr != engine_constants.cend()) {
				this->initial_1i.push_back(std::make_pair(uniform_loc, engconst_itr->second));
			}
		}
	}
	for (GLint index = 0; index < active_attribs; index++) {
		GLsizei written;
		GLint attrib_size;
		GLenum sym_type;
		glGetActiveAttrib(this->program, index, max_read.size(), &written, &attrib_size, &sym_type, max_read.data());
		std::string attrib_name = max_read.substr(0, max_read.find_first_of('\0'));
		std::string s_type{GLSymbol::Get(sym_type).name};
		GLint attrib_loc = this->GetAttributeLocation(attrib_name);
		_log->trace("[Shader] {}/attrib: {}: {} {}: {}", name, attrib_loc, attrib_size, s_type, attrib_name);
	}

	for (auto shader : this->shaders) {
		glDetachShader(this->program, shader);
	}
	return true;
}

void Shader::Use() {
	glUseProgram(this->program);
	if (run_initial) {
		run_initial = false;
		auto _log = spdlog::get("console_log");
		for (auto& initial_set : initial_1i) {
			glUniform1i(initial_set.first, initial_set.second);
			_log->trace("[Shader] Initialize [{}]={}", initial_set.first, initial_set.second);
		}
	}
}

void Shader::UnUse() { glUseProgram(0); }

GLint Shader::GetUniformLocation(const std::string name) {
	auto uniform_itr = this->uniforms.find(name);
	if (uniform_itr != this->uniforms.end()) {
		return uniform_itr->second;
	}
	else {
		GLint uniform_id = glGetUniformLocation(this->program, name.c_str());
		this->uniforms[name] = uniform_id;
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
Shader::CreateFromFile(const std::string name, std::list<std::pair<gfx::ShaderType, Path>> filenames) {
	auto s = std::make_shared<Shader>();
	for (auto pair : filenames) {
		s->LoadFromFile(pair.first, pair.second);
	}
	if (s->Build(name)) {
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
	if (s->Build(name)) {
		ShaderMap::Set(name, s);
		return s;
	}
	return nullptr;
}

void Shader::LoadFromProto(const gfx::ShaderSource& source, gfx::ShaderType type) {
	if (source.type() != gfx::ShaderType::UNSPECIFIED) {
		type = source.type();
	}
	if (type == gfx::ShaderType::UNSPECIFIED) {
		spdlog::get("console_log")->error("[Shader] LoadFromProto no ShaderType specified");
		return;
	}
	switch (source.source_case()) {
	case gfx::ShaderSource::kFile: LoadFromFile(type, Path::GetAssetPath("shaders") / source.file()); break;
	case gfx::ShaderSource::kAbsFile: LoadFromFile(type, Path(source.abs_file())); break;
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
	for (auto& source : shader_def.vertex()) {
		s->LoadFromProto(source, gfx::VERTEX);
	}
	for (auto& source : shader_def.fragment()) {
		s->LoadFromProto(source, gfx::FRAGMENT);
	}
	for (auto& source : shader_def.geometry()) {
		s->LoadFromProto(source, gfx::GEOMETRY);
	}
	for (auto& source : shader_def.tess_control()) {
		s->LoadFromProto(source, gfx::TESS_CONTROL);
	}
	for (auto& source : shader_def.tess_eval()) {
		s->LoadFromProto(source, gfx::TESS_EVAL);
	}
	for (auto& source : shader_def.compute()) {
		s->LoadFromProto(source, gfx::COMPUTE);
	}
	if (s->Build(shader_def.name())) {
		ShaderMap::Set(shader_def.name(), s);
		return s;
	}
	return nullptr;
}

void Shader::IncludeFromDef(const gfx::ShaderInclude& shader_inc) {
	auto _log = spdlog::get("console_log");
	if (shader_inc.name().empty()) {
		_log->error("[Shader::IncludeFromDef] include name must not be empty");
		return;
	}
	auto source = shader_inc.source();
	std::string include_string;
	switch (source.source_case()) {
	case gfx::ShaderSource::kFile: include_string = LoadAsString(Path::GetAssetPath("shaders") / source.file()); break;
	case gfx::ShaderSource::kAbsFile: include_string = LoadAsString(Path(source.abs_file())); break;
	case gfx::ShaderSource::kRaw: include_string = source.raw(); break;
	case gfx::ShaderSource::kByName: _log->error("[Shader] IncludeFromDef by_name not implemented"); break;
	case gfx::ShaderSource::SOURCE_NOT_SET:
	default: break;
	}
	if (include_string.empty()) {
		_log->error("[Shader::IncludeFromDef] include contents must not be empty");
		return;
	}
	ShaderIncludes::Set(shader_inc.name(), std::move(include_string));
}

} // namespace tec
