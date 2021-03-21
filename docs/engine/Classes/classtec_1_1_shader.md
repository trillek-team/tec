---
title: tec::Shader

---

# tec::Shader



## Public Types

|                | Name           |
| -------------- | -------------- |
| enum GLenum | **[ShaderType](/engine/Classes/classtec_1_1_shader/#enum-shadertype)** { VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER, GEOMETRY = GL_GEOMETRY_SHADER} |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Shader](/engine/Classes/classtec_1_1_shader/#function-shader)**() =default |
| | **[~Shader](/engine/Classes/classtec_1_1_shader/#function-~shader)**() |
| void | **[Use](/engine/Classes/classtec_1_1_shader/#function-use)**()<br>Uses the shader program.  |
| void | **[UnUse](/engine/Classes/classtec_1_1_shader/#function-unuse)**()<br>Unuses the shader program.  |
| GLint | **[GetUniformLocation](/engine/Classes/classtec_1_1_shader/#function-getuniformlocation)**(const std::string name)<br>Returns the location of the specified uniform.  |
| GLint | **[GetAttributeLocation](/engine/Classes/classtec_1_1_shader/#function-getattributelocation)**(const std::string name)<br>Returns the location of the specified attribute.  |
| void | **[ActivateTextureUnit](/engine/Classes/classtec_1_1_shader/#function-activatetextureunit)**(const GLuint unit, const GLuint texture_name)<br>Activates the specified texture unit and bind the specified texture to it.  |
| void | **[DeactivateTextureUnit](/engine/Classes/classtec_1_1_shader/#function-deactivatetextureunit)**(const GLuint unit)<br>Deactivates the specified texture unit by binding texture 0 to it.  |

## Public Types Documentation

### enum ShaderType

| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
| VERTEX | GL_VERTEX_SHADER|   |
| FRAGMENT | GL_FRAGMENT_SHADER|   |
| GEOMETRY | GL_GEOMETRY_SHADER|   |




## Public Functions Documentation

### function Shader

```cpp
Shader() =default
```


### function ~Shader

```cpp
~Shader()
```


### function Use

```cpp
void Use()
```

Uses the shader program. 

**Return**: void 

### function UnUse

```cpp
void UnUse()
```

Unuses the shader program. 

**Return**: void 

### function GetUniformLocation

```cpp
GLint GetUniformLocation(
    const std::string name
)
```

Returns the location of the specified uniform. 

**Parameters**: 

  * **const** std::string name The name of the uniform to find the location of. 


**Return**: GLint The location of the requested uniform. 

### function GetAttributeLocation

```cpp
GLint GetAttributeLocation(
    const std::string name
)
```

Returns the location of the specified attribute. 

**Parameters**: 

  * **const** std::string name The name of the attribute to find the location of. 


**Return**: GLint The location of the requested attribute. 

### function ActivateTextureUnit

```cpp
static void ActivateTextureUnit(
    const GLuint unit,
    const GLuint texture_name
)
```

Activates the specified texture unit and bind the specified texture to it. 

**Parameters**: 

  * **const** GLuint unit The texture unit to activate. 
  * **const** GLuint texture_name The GL name of the texture to bind. 


**Return**: void 

### function DeactivateTextureUnit

```cpp
static void DeactivateTextureUnit(
    const GLuint unit
)
```

Deactivates the specified texture unit by binding texture 0 to it. 

**Parameters**: 

  * **const** GLuint unit The texture unit to deactivate. 


**Return**: void 

-------------------------------

Updated on 21 March 2021 at 16:58:09 UTC