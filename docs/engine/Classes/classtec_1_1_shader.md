---
title: tec::Shader

---

# tec::Shader





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Shader](/engine/Classes/classtec_1_1_shader/#function-shader)**() =default |
| | **[~Shader](/engine/Classes/classtec_1_1_shader/#function-~shader)**() |
| void | **[Use](/engine/Classes/classtec_1_1_shader/#function-use)**()<br>Uses the shader program.  |
| void | **[UnUse](/engine/Classes/classtec_1_1_shader/#function-unuse)**()<br>Unuses the shader program.  |
| GLint | **[GetUniformLocation](/engine/Classes/classtec_1_1_shader/#function-getuniformlocation)**(const std::string name)<br>Returns the location of the specified uniform.  |
| GLint | **[GetAttributeLocation](/engine/Classes/classtec_1_1_shader/#function-getattributelocation)**(const std::string name)<br>Returns the location of the specified attribute.  |

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

-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000