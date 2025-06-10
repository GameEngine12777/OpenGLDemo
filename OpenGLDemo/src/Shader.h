#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	// Set uniform
	void SetUniform1i(const std::string& name, int v1);
	void SetUniform1f(const std::string& name, float v1);
	void SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4);

	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	int GetUniformLocation(const std::string& name) const;

	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CreateShader(const std::string& VertexShader, const std::string& FragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
};

