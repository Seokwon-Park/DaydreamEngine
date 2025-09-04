#pragma once



namespace Daydream
{
	class Shader;
	class ShaderGroup;
	
	class ShaderManager
	{
	public:
		ShaderManager();

		Shared<Shader> Get(const String& _name) 	
		{
			return shaderCache[_name];
		}

		Shared<ShaderGroup> GetShaderGroup(const String& _name) 
		{
			return shaderGroupCache[_name];
		}

		void LoadShadersFromDirectory(Path _directory, bool _isRecursive = false);
		void CreateEssentialShaderGroups();
	private:
		HashMap<String, Shared<Shader>> shaderCache;
		HashMap<String, Shared<ShaderGroup>> shaderGroupCache;
	};
}