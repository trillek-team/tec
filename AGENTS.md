# AI Agent Instructions for Trillek Engine C (TEC)

**Purpose**: Internal process guidelines for AI agents working on TEC  
**Audience**: AI assistants only (not user documentation)  
**Status**: Active

---

## ⚠️ MANDATORY FIRST STEPS - READ THIS BEFORE ANY ACTION ⚠️

**CRITICAL**: Follow these steps IN ORDER before attempting ANY build or code work. Skipping steps will cause build failures.

### Complete Setup Checklist

**For All Builds:**
1. ✅ Install system dependencies
2. ✅ Clone and bootstrap vcpkg
3. ✅ Set VCPKG_ROOT environment variable
4. ✅ Install CMake 3.20+ and Ninja
5. ✅ Run CMake configure
6. ✅ Build

---

### Step 1: Install System Dependencies

**Linux**:
```bash
# Install ALL dependencies in one command
sudo apt-get update && sudo apt-get install -y \
  build-essential \
  pkg-config \
  tar \
  curl \
  zip \
  unzip \
  gdb \
  cmake \
  ninja-build \
  libgl1-mesa-dev \
  xorg-dev \
  libglu1-mesa-dev \
  libxinerama-dev \
  libxcursor-dev \
  p7zip-full
```

**What these are for**:
- `build-essential` - GCC/G++ 10+ and build tools
- `cmake` - Build system generator (3.20+ required)
- `ninja-build` - Fast build tool (required by CMake presets)
- `pkg-config` - Library detection
- `libgl1-mesa-dev`, `libglu1-mesa-dev` - OpenGL libraries
- `xorg-dev`, `libxinerama-dev`, `libxcursor-dev` - X11 libraries for GLFW3
- `p7zip-full`, `tar`, `curl`, `zip`, `unzip` - Archive/download tools for vcpkg

**Compiler Requirements**:
- Linux: `gcc10/g++10` or `clang-13` minimum (C++20 support)
- Windows: Visual Studio 2019+ with C++ tools
- macOS: Xcode Command Line Tools

---

### Step 2: Clone and Bootstrap vcpkg

```bash
# Clone vcpkg (can be anywhere)
git clone https://github.com/microsoft/vcpkg.git

# Bootstrap vcpkg
cd vcpkg
./bootstrap-vcpkg.sh      # Linux/macOS
bootstrap-vcpkg.bat       # Windows

# Verify bootstrap
./vcpkg --version  # Should show vcpkg version
```

---

### Step 3: Set Environment Variables

**Linux/macOS**:
```bash
# Set VCPKG_ROOT environment variable (REQUIRED)
export VCPKG_ROOT=/path/to/vcpkg  # Adjust to your vcpkg location

# Verify
echo $VCPKG_ROOT    # Should show path
```

**Windows (CMD)**:
```cmd
set VCPKG_ROOT=C:\path\to\vcpkg
```

**Windows (PowerShell)**:
```powershell
$env:VCPKG_ROOT = "C:\path\to\vcpkg"
```

---

### Step 4: Configure CMake

**IMPORTANT**: Only run configure once unless you need to reconfigure.

**Using Ninja Multi-Config (Recommended)**:
```bash
cd tec
cmake --preset=ninja-multi-vcpkg
```

This will take 5-15 minutes the first time as vcpkg builds all dependencies (protobuf, bullet3, glfw3, imgui, openal, lua, sol3, spdlog, etc.).

---

### Step 5: Build

```bash
# Debug build
cmake --build --preset cli-native-debug --parallel 4

# Release build
cmake --build --preset cli-native-release --parallel 4
```

---

### Platform Triplets Reference

- `x64-windows` - Windows native (must specify on command line)
- `x64-linux` - Linux native (Clang-18+)
- `x64-osx` - macOS native

---

### Using setup-cpp for Automated Setup (Web Coding Environments)

If you're in a web-based coding environment like GitHub Codespaces, use the `aminya/setup-cpp` action to automate tool installation:

```yaml
# In GitHub Actions or Codespaces
- name: Setup C++ environment
  uses: aminya/setup-cpp@v1
  with:
    compiler: clang
    clang: 18
    cmake: true
    ninja: true
    vcpkg: true
```

This automatically installs Clang-18, CMake, Ninja, and vcpkg without manual steps.

---

**These steps are NOT optional. Complete ALL steps IN ORDER before proceeding with any task.**

---

## Core Directives

### 1. Required Reading Before Any Work

**Read CODE_STYLE_GUIDE.md for coding standards** before making changes.

**Future pattern guides**:
- ImGui UI patterns and immediate mode GUI architecture
- ECS patterns with the engine's entity-component system
- Networking patterns with protobuf serialization
- Lua scripting API usage

These guides will define correct implementation patterns. Don't improvise - follow documented patterns when they exist.

### 2. Planning is Internal Only

- **Never create plan documentation files** unless explicitly requested
- Plans, strategies, and implementation approaches are working memory only
- Execute directly without documenting the plan
- Exception: AGENTS.md (this file) is allowed as it improves AI process

### 3. Temporary Files Must Be Cleaned Up

- **Never leave temporary test programs, scripts, or exploration code** in the repository
- If you create temporary files for investigation (e.g., testing a hypothesis, exploring an API):
    - Delete them immediately after use
    - Use `.gitignore`d locations if they must persist briefly
- Clean up at the end of your task before reporting completion
- Examples of temporary files to avoid: test programs, debug scripts, scratch files

### 4. Documentation Policy

**NEVER create summary documents, reports, or reviews** of work completed. These are working memory only.

**Do NOT create** unless explicitly requested:

- HOWTOs, tutorials, guides
- Implementation summaries, work summaries, review summaries
- Design notes or architecture docs
- Supplementary documentation of any kind
- Status reports, completion reports, or any "summary" documents

**Only create NEW documentation if**:

- User explicitly requests it (e.g., "create a document explaining X")
- It documents a human workflow/process users must follow (e.g., build instructions, setup steps)
- It directly improves automated tooling or AI code generation

**When creating NEW documentation files**:

- You MUST provide a justification in your response explaining:
    - Why this needs to be a persistent file vs. chat response
    - What specific future use case requires this reference
    - How it differs from existing documentation
- Updating existing documentation does NOT require justification

**Rule of thumb**: If you're just going to tell the user about it anyway, don't create a file for it.

**CRITICAL**: If the justification for creating documentation is NOT immediately obvious:
1. **DO NOT create the file**
2. **Explain your reasoning to the user first**
3. **Wait for explicit approval**
4. If unclear whether documentation is needed, **update AGENTS.md to clarify the policy** rather than guess

### 5. No Standalone Applications

- Do not create standalone test applications
- Test features by integrating into existing code (e.g., main menu)

---

## Execution Protocol

### Before Any Task

1. Read request carefully
2. Identify files to modify (surgical changes only)
3. Execute changes directly without creating plan documents
4. Build and test to verify changes

### Making Changes

- **Minimal modifications only**: Change as few lines as possible
- Do not delete/remove/modify working code unless absolutely necessary
- Ignore unrelated bugs or broken tests
- Update documentation only if directly related to changes

### Build Verification (REQUIRED)

**Build/test MUST succeed before completing any task.**

**Compiler Requirements**:
- **Linux**: gcc10/g++10 or clang-13 minimum (C++20 support)
- **Windows**: Visual Studio 2019+ with C++ tools
- **macOS**: Xcode Command Line Tools

**System Dependencies** (already covered in Step 1 above)

Before completing any work:

1. **Ensure VCPKG_ROOT is set** (see Step 3 above)

2. **Configure** (first time only):
   ```bash
   cmake --preset=ninja-multi-vcpkg
   ```

3. **Build**:
   ```bash
   # Debug build
   cmake --build --preset=ninja-multi-vcpkg-debug
   
   # Release build
   cmake --build --preset=ninja-multi-vcpkg-release
   ```

4. **Verify**: No errors or warnings
5. **If errors**: Fix and rebuild
6. **Do not mark complete until build succeeds**

**Build Targets**:
- Client executable in `builds/ninja-multi-vcpkg/client/`
- Server executable in `builds/ninja-multi-vcpkg/server/`
- Common library linked by both

### Test Verification (REQUIRED)

After successful build, run tests before completing work:

1. **Configure tests (first time)**:
   ```bash
   cmake --preset=ninja-multi-vcpkg -DBUILD_TESTS=ON
   ```

2. **Build tests**:
   ```bash
   cmake --build --preset=ninja-multi-vcpkg-debug
   ```

3. **Run all tests**:
   ```bash
   ctest --preset=ninja-multi-vcpkg-debug
   ```

   Or run directly:
   ```bash
   cd builds/ninja-multi-vcpkg
   ctest -C Debug --output-on-failure
   ```

4. **Verify**: All tests pass (100% success rate)
5. **If tests fail**:
    - Only fix failures related to your changes
    - Ignore pre-existing test failures unrelated to your work
    - Document any known issues
6. **Do not mark complete until tests pass**

**Note**: See `TESTING.md` for detailed test documentation and advanced usage.

---

## Project Constraints

### Technical Requirements

- **C++20**: Strict requirement
- **CMake 3.28+**: Required
- **vcpkg**: Use for dependencies
- **ImGui**: UI framework (with GLFW + OpenGL3 bindings)
- **GLFW3**: Windowing and input
- **OpenGL**: Rendering backend
- **flecs**: ECS framework

### Platform Support

- Windows: MSVC or MinGW-w64 GCC 10+
- Linux: GCC 10+ or Clang 10+
- macOS: Xcode 12+ / Clang 10+

### Code Standards

#### Modern C++20 Requirements

- **C++20 features**: Use ranges, concepts, coroutines (when appropriate)
- **RAII**: All resource management (files, memory, locks) via RAII types
- **Smart pointers**:
    - `std::unique_ptr` for exclusive ownership
    - `std::shared_ptr` only when truly shared ownership needed
    - Raw pointers for non-owning references
    - Never use `new`/`delete` directly
- **Containers**: Prefer standard library containers over manual memory management
- **Algorithms**: Use `<algorithm>` and `<ranges>` over raw loops when clearer

#### Modern C++ Patterns

```cpp
// CORRECT: Use ranges instead of raw loops
auto filtered = data 
    | std::views::filter([](const auto& item) { return item.active; })
    | std::views::transform([](const auto& item) { return item.value; });

// CORRECT: Smart pointers for ownership
auto entity = std::make_unique<Entity>();
scene->AddEntity(std::move(entity));  // Transfer ownership

// CORRECT: RAII for resources
class ResourceManager {
    std::unique_ptr<Resource> resource_;  // Automatically cleaned up
};

// CORRECT: Small, focused functions
auto FindEntity(int id) const -> std::optional<Entity> {
    auto it = std::ranges::find_if(entities_, 
        [id](const auto& e) { return e.id == id; });
    return it != entities_.end() ? std::optional{*it} : std::nullopt;
}
```

#### Composition Over Inheritance

- **Prefer composition** for building functionality
- **Use inheritance** only for polymorphism (Gang of Four patterns like Strategy, Observer, Composite)
- Keep inheritance hierarchies shallow (max 2-3 levels)

```cpp
// CORRECT: Composition for building features
class GameWorld {
    SceneManager scene_manager_;        // Has-a
    EntityRegistry entity_registry_;    // Has-a
    SystemManager system_manager_;      // Has-a
};

// CORRECT: Inheritance for polymorphism (Strategy pattern)
class System {
public:
    virtual void Update(float delta_time) = 0;
    virtual void Render() const = 0;
};

class PhysicsSystem : public System { /* ... */ };
class RenderSystem : public System { /* ... */ };

// WRONG: Inheritance for code reuse
class PlayerEntity : public Entity {  // Avoid - use composition/ECS instead
};
```

#### Gang of Four Patterns (Use When Appropriate)

- **Observer**: Event callbacks, reactive updates
- **Strategy**: Interchangeable algorithms (rendering backends, input handlers)
- **Factory**: Entity/component creation in ECS
- **Command**: Action history, undo/redo systems
- **Singleton**: Use sparingly, only for true single instances (engine core, resource managers)

#### Code Organization

- **Small functions**: Max 20-30 lines, single responsibility
- **Comments**: Only for complex logic or non-obvious design decisions
- **Const correctness**: Use `const` everywhere possible
- **Type safety**: Prefer `enum class` over plain `enum` or integers

---

## File Organization

### Source Structure

- `client/` - Client-specific code (rendering, GUI, input)
- `server/` - Server-specific code (networking, simulation)
- `common/` - Shared code (ECS, physics, events, resources)
- `tests/` - Unit and integration tests (GTest)
- `docs/` - User-facing documentation (mkdocs format)
  - `docs/engine/` - Engine API docs (generated by doxybook2, committed)
- `assets/` - Game assets (textures, models, shaders)
- `proto/` - Protocol buffer definitions

### Documentation Files

- `README.md` - Build instructions, quick start
- `CONTRIBUTING.md` - Contribution guidelines
- `CODE_STYLE_GUIDE.md` - Coding standards (THIS FILE defines style)
- `AGENTS.md` - AI agent instructions (this file)
- `docs/` - Mkdocs user documentation

---

## Documentation Guidelines

### User Documentation (README.md)

- Focus on build instructions and quick start
- User-oriented content (how to build, run, contribute)
- Not implementation details or architecture discussions

### Code Comments

- Only for complex logic explanation or non-obvious design decisions
- Don't comment on "what" changed - that's in version control
- Don't explain obvious code
- Self-documenting code preferred

---

## Process Checklist

When given a task:

- [ ] Understand requirement (no plan doc needed)
- [ ] Identify minimal changes
- [ ] Execute changes directly
- [ ] Build and verify
- [ ] Clean up any temporary files created during investigation
- [ ] No standalone tests/demos created
- [ ] No unnecessary documentation created
- [ ] Changes are surgical and minimal

---

## Error Handling

### Build Errors

- Fix immediately
- Rebuild to verify
- Do not proceed until clean build

### Breaking Changes

- Avoid at all costs
- Opt-in features only
- Maintain backward compatibility

---

## Key Principles

1. **Execute, don't document the plan**
2. **Minimal changes, surgical precision**
3. **Test in production code, not standalone apps**
4. **Build verification is mandatory**
5. **No supplementary documentation**
6. **When in doubt, ask the user**

---

**Status**: This file is the only internal process documentation permitted. Follow these directives strictly.
