#pragma once

#include <memory>
#include <map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Renderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

// Holds all state information relevant to a character as loaded using FreeType
struct Character 
{
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

class Graphics
{
public:
	Graphics();
	~Graphics();

	void SetProjectionMatrix(const glm::mat4& projection);
	void SetViewMatrix(const glm::mat4& view);
	void ClearColor();
	void DrawInstancedTexture(const void* staticData, unsigned int staticDataSize, const void* dynamicData, unsigned int dynamicDataSize, const Texture* texture, int instanceCount);
	void DrawStaticTexture(const void* staticData, unsigned int staticDataSize, const Texture* texture);

	void DrawString(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

private:
	glm::mat4 m_Projection, m_View;
	std::unique_ptr<Shader> m_InstancedShader;
	std::unique_ptr<Shader> m_BasicShader;
	std::unique_ptr<Shader> m_TextShader;

	std::map<GLchar, Character> Characters;

	void EnableBlending();
};




