#pragma once

#include <fmt/core.h>
#include <forward_list>
#include <map>

namespace tec::graphics {

class GLSymbol {
public:
	static const GLSymbol& Get(GLenum which) {
		static const std::map<GLenum, GLSymbol> symbolic_gl_types{
				// error codes
				{GL_NO_ERROR, {"GL_NO_ERROR"}},
				{GL_INVALID_ENUM, {"GL_INVALID_ENUM"}},
				{GL_INVALID_VALUE, {"GL_INVALID_VALUE"}},
				{GL_INVALID_OPERATION, {"GL_INVALID_OPERATION"}},
				{GL_INVALID_FRAMEBUFFER_OPERATION, {"GL_INVALID_FRAMEBUFFER_OPERATION"}},
				{GL_OUT_OF_MEMORY, {"GL_OUT_OF_MEMORY"}},
				// image formats
				{GL_DEPTH_COMPONENT, {"GL_DEPTH_COMPONENT"}},
				{GL_DEPTH_STENCIL, {"GL_DEPTH_STENCIL"}},
				{GL_RED, {"GL_RED"}},
				{GL_RG, {"GL_RG"}},
				{GL_RGB, {"GL_RGB"}},
				{GL_RGBA, {"GL_RGBA"}},
				{GL_SRGB, {"GL_SRGB"}},
				{GL_SRGB_ALPHA, {"GL_SRGB_ALPHA"}},
				{GL_SRGB8_ALPHA8, {"GL_SRGB8_ALPHA8"}},
				// shader types
				{GL_FLOAT, {"float"}},
				{GL_FLOAT_VEC2, {"vec2"}},
				{GL_FLOAT_VEC3, {"vec3"}},
				{GL_FLOAT_VEC4, {"vec4"}},
				{GL_INT, {"int"}},
				{GL_INT_VEC2, {"ivec2"}},
				{GL_INT_VEC3, {"ivec3"}},
				{GL_INT_VEC4, {"ivec4"}},
				{GL_SAMPLER_1D, {"sampler1D"}},
				{GL_SAMPLER_2D, {"sampler2D"}},
				{GL_SAMPLER_3D, {"sampler3D"}},
				{GL_SAMPLER_2D_SHADOW, {"sampler2DShadow"}},
				{GL_SAMPLER_CUBE_SHADOW, {"samplerCubeShadow"}},
				{GL_SAMPLER_2D_ARRAY, {"sampler2DArray"}},
				{GL_UNSIGNED_INT, {"uint"}},
				{GL_FLOAT_MAT4, {"mat4x4"}},
		};
		static std::forward_list<std::string> unknown_gl_strings;
		static std::map<GLenum, GLSymbol> unknown_gl_types;
		if (const auto sym_itr = symbolic_gl_types.find(which); sym_itr != symbolic_gl_types.cend()) {
			return sym_itr->second;
		}
		unknown_gl_strings.push_front(std::to_string(which));
		return unknown_gl_types.emplace(std::make_pair(which, GLSymbol{unknown_gl_strings.front()})).first->second;
	}
	std::string name;
};
} // namespace tec::graphics

template <> struct fmt::formatter<tec::graphics::GLSymbol> {
	static constexpr auto parse(const format_parse_context& ctx) -> decltype(ctx.begin()) {
		const auto it = ctx.begin();
		if (const auto end = ctx.end(); it != end && *it != '}')
			throw std::runtime_error("invalid format");
		return it;
	}
	template <typename FormatContext>
	static auto format(const tec::graphics::GLSymbol& p, FormatContext& ctx) -> decltype(ctx.out()) {
		return fmt::format_to(ctx.out(), "{}", p.name);
	}
};
