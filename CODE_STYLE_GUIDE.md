# Trillek Engine C (TEC) - C++20 Code Style Guide

> **Consistent coding standards for the Trillek Engine C project**

## Overview

This document defines the coding standards and style guidelines for Citrus Engine. Consistency in code style
improves readability, maintainability, and collaboration.

## Naming Conventions

### Functions and Methods

- **Style**: PascalCase
- **Examples**:
  ```cpp
  void InitializeRenderer();
  bool LoadTexture(const std::string& path);
  Mat4 GetViewMatrix() const;
  ```

### Variables and Parameters

- **Style**: snake_case
- **Examples**:
  ```cpp
  int entity_count;
  float delta_time;
  const std::string& file_path;
  ```

### Classes and Structs

- **Style**: PascalCase
- **Examples**:
  ```cpp
  class EntityManager;
  struct Transform;
  class TextureManager;
  ```

### Enums and Enum Values

- **Style**: PascalCase for enum class, PascalCase for values
- **Examples**:
  ```cpp
  enum class ProjectionType {
      Perspective,
      Orthographic
  };
  
  enum class TextureFormat {
      RGB8,
      RGBA8,
      RGBA16F
  };
  ```

### Constants

- **Style**: PascalCase for const variables, UPPER_CASE for macros
- **Examples**:
  ```cpp
  constexpr float DefaultFov = 60.0f;
  const Color WhiteColor{1.0f, 1.0f, 1.0f, 1.0f};
  
  #define ENGINE_VERSION_MAJOR 1
  ```

### Member Variables

- **Style**: snake_case with trailing underscore for private members
- **Examples**:
  ```cpp
  class Camera {
  public:
      Vec3 position{0.0f};  // Public members: no underscore
      
  private:
      Vec3 position_{0.0f}; // Private members: trailing underscore
      bool view_dirty_ = true;
  };
  ```

### Namespaces

- **Style**: snake_case
- **Examples**:
  ```cpp
  namespace engine::platform {}
  namespace engine::rendering {}
  ```

### Template Parameters

- **Style**: PascalCase
- **Examples**:
  ```cpp
  template<typename ComponentType>
  template<Component T>
  template<size_t BufferSize>
  ```

## File Naming

### Header Files

- **Style**: kebab-case with .hpp extension
- **Examples**: `entity-manager.hpp`, `physics-system.hpp`, `render-system.hpp`

### Source Files

- **Style**: kebab-case with .cpp extension
- **Examples**: `entity-manager.cpp`, `physics-system.cpp`

### Directory Structure

- **Style**: lowercase, sometimes with underscores
- **Examples**: `client/`, `server/`, `common/`, `docs_targets/`

## Code Formatting

### Indentation

- **4 spaces** (no tabs)
- Consistent indentation for all nested blocks

### Braces

- **K&R style** for all situations (opening brace on same line)

```cpp
class MyClass {
public:
    void MyFunction() {
        if (condition) {
            // Do something
        } else {
            // Do something else
        }
        
        for (int i = 0; i < count; ++i) {
            // Loop body
        }
    }
};
```

### Line Length

- **Maximum 120 characters** per line
- Break long lines at logical points (parameters, operators)

### Spacing

- Space after keywords: `if (`, `for (`, `while (`
- Space around operators: `a + b`, `x == y`
- No space before semicolons: `func();`
- Space after commas: `func(a, b, c);`

## C++20 Specific Guidelines

### Headers (Traditional)

- Use `#pragma once` for header guards (preferred over `#ifndef`)
- Include order: own header, standard library, third-party, project headers
- Use forward declarations where possible to reduce compile times

```cpp
#pragma once

#include <vector>  // Standard library
#include <memory>

#include <glm/glm.hpp>  // Third-party

#include "entity.hpp"    // Project headers
#include "components/transform.hpp"
```

### Concepts

- Use PascalCase for concept names
- Place concepts before template declarations

```cpp
template<typename T>
concept Component = std::is_trivially_copyable_v<T> && std::is_standard_layout_v<T>;

template<Component T>
void RegisterComponent();
```

### Auto and Type Deduction

- Use `auto` for complex type names and lambda expressions
- Avoid `auto` when type clarity is important

```cpp
auto texture_manager = GetTextureManager();  // Good: complex type
auto count = 5;                              // Bad: unclear type
int count = 5;                               // Good: clear intent
```

## Documentation

### Comments

- Use `//` for single-line comments
- Use `/* */` for multi-line comments
- Document public interfaces with brief descriptions

```cpp
/// Get the current frame rate in frames per second
/// @return Current FPS as a double value
double GetCurrentFps() const;

/**
 * Create a new texture from image file
 * @param path File path to the image
 * @param format Desired texture format
 * @return Texture ID or INVALID_TEXTURE on failure
 */
TextureId CreateTexture(const fs::Path& path, TextureFormat format);
```

### Module Documentation

- Each module interface should have a header comment explaining its purpose
- Document major design decisions and constraints

## Error Handling

### Return Values

- Use meaningful return types (not just bool)
- Prefer exceptions for exceptional cases
- Use `std::optional` for operations that may fail

```cpp
std::optional<TextureId> LoadTexture(const fs::Path& path);
void InitializeRenderer();  // Throws on critical failure
bool IsValid(TextureId id) const;  // Simple validation
```

### Assertions

- Use assertions for development-time checks
- Use graceful error handling for runtime errors

```cpp
assert(entity.IsValid());  // Development check
if (!file.IsOpen()) {      // Runtime error handling
    return std::nullopt;
}
```

## Performance Guidelines

### Memory Management

- Prefer stack allocation over heap allocation
- Use smart pointers for ownership management
- Consider custom allocators for high-frequency allocations

### Function Design

- Pass large objects by const reference
- Return small objects by value
- Use move semantics for expensive-to-copy objects

```cpp
void ProcessEntity(const Transform& transform);     // Large object by const ref
Vec3 GetPosition() const;                           // Small object by value
std::unique_ptr<Texture> CreateTexture(TextureCreateInfo&& info);  // Move semantics
```

## Example: Well-Formatted Code

```cpp
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace tec {

    class TextureManager {
    public:
        TextureManager();
        ~TextureManager();
        
        /// Create a new texture from the provided creation info
        TextureId CreateTexture(const TextureCreateInfo& info);
        
        /// Load texture from file path
        std::optional<TextureId> LoadTexture(const std::string& path);
        
        /// Get texture dimensions
        glm::vec2 GetTextureDimensions(TextureId id) const;
        
        /// Check if texture ID is valid
        bool IsValidTexture(TextureId id) const;
        
    private:
        std::unordered_map<TextureId, TextureData> textures_;
        TextureId next_id_ = 1;
        
        bool ValidateTextureData(const TextureCreateInfo& info) const;
    };

} // namespace tec
```

## Project-Specific Patterns

### Entity-Component-System (ECS)

TEC uses a custom ECS architecture. Follow these patterns:

```cpp
// Components are POD structs
struct Transform {
    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 scale{1.0f, 1.0f, 1.0f};
};

// Entities are simple ID types
using EntityId = std::uint64_t;

// Systems operate on components
class PhysicsSystem {
public:
    void Update(double delta_time);
private:
    // System implementation
};
```

### Protobuf Serialization

Network messages use Protocol Buffers. Place `.proto` files in `common/proto/`.

### Lua Scripting

TEC integrates Lua via sol3. Script bindings go in dedicated `*-lua.cpp` files.

### ImGui Integration

Debug UI uses Dear ImGui with immediate mode patterns. See `imgui-system.cpp` for examples.

This style guide ensures consistency across the TEC codebase while leveraging modern C++20 features effectively.
