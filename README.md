# DaydreamEngine
<img width="300" height="300" alt="Daydream Icon" src="https://github.com/user-attachments/assets/ab7b46c6-c115-482a-bb7c-4e57d9e7350c" />

## Features
- **Traditional Entity-Component Architecture**
- **Cross-Platform Support:** Powered by GLFW (Currently Windows 10)
- **Multi-Backend Rendering:**
  - OpenGL 4.5
  - DirectX 11 & 12
  - Vulkan 1.4
- **Unified Shader Workflow:** Write once in HLSL, cross-compiled via SPIRV-Cross for all backends.

## Environment
This project has been developed and tested in the following environment:

* **OS:** Windows 10 (64-bit)
* **IDE:** Visual Studio 2022
* **GPU Tested:** NVIDIA Geforce RTX 2070 SUPER
  
## Prerequisites
To build and run the engine, you need:

* **Windows 10**
* **Visual Studio 2022** (Desktop development with C++)
* **SDKs:**
    * **Vulkan SDK:** 1.4.321.1 or higher (Required for Vulkan backend)
    * **Windows SDK:** Latest version (Required for DX11/12)

## Getting Started

### 1. Clone the Repository
Use the `--recursive` flag to verify that all dependencies are loaded correctly.

Clone the repository
```bash
git clone --recursive https://github.com/Seokwon-Park/DaydreamEngine
```

If you have already cloned the repository non-recursively, you can initialize the submodules with the following command:
```bash
git submodule update --init --recursive
```

### 2. Generate Project Files
Run the script to generate the Visual Studio solution files.

Execute Win-GenProjects.bat (Windows Only).

Open the generated solution file (e.g., DaydreamEngine.sln) in Visual Studio 2022.

Set DaydreamEngine as the startup project and build.

