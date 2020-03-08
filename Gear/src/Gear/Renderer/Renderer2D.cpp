#include "grpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

#include "glad/glad.h"

namespace Gear {

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuardVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		GR_PROFILE_FUNCTION();

		s_Data = new Renderer2DStorage();

		//1. Create VertexArray
		s_Data->QuardVertexArray = VertexArray::Create();
		//2. define vertices, Indices
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		uint32_t squreIndices[6] = {
			0, 1, 2, 2, 3, 0
		};

		//3. Create VertexBuffer
		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
		});
		s_Data->QuardVertexArray->AddVertexBuffer(squareVB);

		//4. Create IndexBuffer
		Ref<IndexBuffer> squreIB;
		squreIB.reset(IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t)));
		s_Data->QuardVertexArray->SetIndexBuffer(squreIB);

		//5. Shader setup
		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture0", 0);
		s_Data->TextureShader->SetInt("u_Texture1", 1);
		s_Data->TextureShader->SetInt("u_Texture2", 2);
	}

	void Renderer2D::Shutdown()
	{
		GR_PROFILE_FUNCTION();

		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		GR_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		GR_PROFILE_FUNCTION();
		
	}

	std::tuple<int, int, int> Renderer2D::getPixel(int x, int y)
	{
		unsigned char* data = new unsigned char[1];

		glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);

		std::tuple<int, int, int> ret = { data[0], data[1], data[2] };

		return ret;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		GR_PROFILE_FUNCTION();

		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data->WhiteTexture->Bind();

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position) 
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", translate);

		s_Data->QuardVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuardVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3 & position, const glm::vec2 & size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		GR_PROFILE_FUNCTION();

		s_Data->TextureShader->SetFloat4("u_Color", tintColor);
		s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position) 
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", translate);

		s_Data->QuardVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuardVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2 & position, const glm::vec2 & size, float rotation, const glm::vec4 & color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3 & position, const glm::vec2 & size, float rotation, const glm::vec4 & color)
	{
		GR_PROFILE_FUNCTION();

		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data->WhiteTexture->Bind();

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f, })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", translate);

		s_Data->QuardVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuardVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2 & position, const glm::vec2 & size, float rotation, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor, int slot, int x, int y)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor, slot);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3 & position, const glm::vec2 & size, float rotation, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor, int slot, int x, int y)
	{
		GR_PROFILE_FUNCTION();

		s_Data->TextureShader->SetFloat4("u_Color", tintColor);
		s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind(x, y);

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f, })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", translate);

		s_Data->QuardVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuardVertexArray);
	}

}