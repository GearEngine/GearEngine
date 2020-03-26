#include "grpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

#include "glad/glad.h"

namespace Gear {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		//TODO : color, texid, maskid
	};

	struct Renderer2DData
	{
		//Batch
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;

		Ref<VertexArray> QuardVertexBatchArray;
		Ref<VertexBuffer> QuardVertexBatchBuffer;
		
		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		Ref<Shader> BatchShader;

		//Legacy
		Ref<VertexArray> QuardVertexArray;
		Ref<Shader> TextureShader;
		Ref<Shader> FixedShader;
		Ref<Texture2D> WhiteTexture;
	};
	
	static Renderer2DData s_Data;

	

	void Renderer2D::Init()
	{
		GR_PROFILE_FUNCTION();

		//1. Create VertexArray
		s_Data.QuardVertexArray = VertexArray::Create();
		s_Data.QuardVertexBatchArray = VertexArray::Create();

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

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		//3. Create VertexBuffer
		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
		});
		s_Data.QuardVertexArray->AddVertexBuffer(squareVB);


		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
		s_Data.QuardVertexBatchBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuardVertexBatchBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		});
		s_Data.QuardVertexBatchArray->AddVertexBuffer(s_Data.QuardVertexBatchBuffer);

		//4. Create IndexBuffer
		Ref<IndexBuffer> squreIB = IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t));
		s_Data.QuardVertexArray->SetIndexBuffer(squreIB);

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuardVertexBatchArray->SetIndexBuffer(quadIB);

		//5. Shader setup
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_Texture", 0);
		s_Data.TextureShader->SetInt("u_Mask", 1);

		s_Data.FixedShader = Shader::Create("assets/shaders/Fixed.glsl");
		s_Data.FixedShader->Bind();
		s_Data.FixedShader->SetInt("u_Texture", 0);

		s_Data.BatchShader = Shader::Create("assets/shaders/Batch.glsl");
		s_Data.BatchShader->Bind();
		s_Data.BatchShader->SetInt("u_Texture", 0);

		delete[] quadIndices;
	}

	void Renderer2D::Shutdown()
	{
		GR_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		GR_PROFILE_FUNCTION();

		s_Data.BatchShader->Bind();
		s_Data.BatchShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{
		GR_PROFILE_FUNCTION();
		
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuardVertexBatchBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::DrawTextureWithMask(const glm::mat4 & translate, const Ref<Texture>& texture, const Ref<Texture>& mask)
	{
		s_Data.TextureShader->Bind();
		texture->Bind(0);
		mask->Bind(1);

		s_Data.TextureShader->SetInt("u_WithMask", 1);
		s_Data.TextureShader->SetMat4("u_Transform", translate);

		s_Data.QuardVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuardVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		GR_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_WithMask", 0);
		s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data.WhiteTexture->Bind();

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position) 
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", translate);

		s_Data.QuardVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuardVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3 & position, const glm::vec2 & size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		GR_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_WithMask", 0);
		s_Data.TextureShader->SetFloat4("u_Color", tintColor);
		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position) 
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", translate);

		s_Data.QuardVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuardVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::mat4 & translate, const glm::vec4 & color)
	{
		GR_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_WithMask", 0);
		s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data.TextureShader->SetMat4("u_Transform", translate);

		s_Data.WhiteTexture->Bind();
		s_Data.QuardVertexArray->Bind();

		RenderCommand::DrawIndexed(s_Data.QuardVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::mat4 & translate, const Ref<Texture>& texture, const glm::vec4 & tintColor)
	{
		GR_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_WithMask", 0);
		s_Data.TextureShader->SetFloat4("u_Color", tintColor);
		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data.TextureShader->SetMat4("u_Transform", translate);
		
		texture->Bind();
		s_Data.QuardVertexArray->Bind();

		RenderCommand::DrawIndexed(s_Data.QuardVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2 & position, const glm::vec2 & size, float rotation, const glm::vec4 & color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3 & position, const glm::vec2 & size, float rotation, const glm::vec4 & color)
	{
		GR_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_WithMask", 0);
		s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data.WhiteTexture->Bind();

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f, })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", translate);

		s_Data.QuardVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuardVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2 & position, const glm::vec2 & size, float rotation, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3 & position, const glm::vec2 & size, float rotation, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		GR_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_WithMask", 0);
		s_Data.TextureShader->SetFloat4("u_Color", tintColor);
		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f, })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", translate);

		s_Data.QuardVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuardVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::mat4 & translate, const glm::vec4 & color)
	{
		GR_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_WithMask", 0);
		s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data.TextureShader->SetMat4("u_Transform", translate);
	
		s_Data.WhiteTexture->Bind();
		s_Data.QuardVertexArray->Bind();

		RenderCommand::DrawIndexed(s_Data.QuardVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::mat4 & translate, const Ref<Texture>& texture, const glm::vec4 & tintColor)
	{
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_WithMask", 0);
		s_Data.TextureShader->SetFloat4("u_Color", tintColor);
		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data.TextureShader->SetMat4("u_Transform", translate);

		texture->Bind();
		s_Data.QuardVertexArray->Bind();

		RenderCommand::DrawIndexed(s_Data.QuardVertexArray);
	}

	void Renderer2D::DrawAnimationQuad(const glm::vec2 & position, const glm::vec2 & size, const Ref<Animation2D> animation, const glm::vec4 & tintColor)
	{
		DrawAnimationQuad({ position.x, position.y, 0.0f }, size, animation, tintColor);
	}

	void Renderer2D::DrawAnimationQuad(const glm::vec3 & position, const glm::vec2 & size, const Ref<Animation2D> animation, const glm::vec4 & tintColor)
	{
		GR_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_WithMask", 0);
		s_Data.TextureShader->SetFloat4("u_Color", tintColor);
		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		animation->Bind();

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", translate);

		s_Data.QuardVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuardVertexArray);
	}

	void Renderer2D::DrawAnimationRotateQuad(const glm::vec2 & position, const glm::vec2 & size, float rotation, const Ref<Animation2D> animation, const glm::vec4 & tintColor)
	{
		DrawAnimationRotateQuad({ position.x, position.y, 0.0f }, size, rotation, animation, tintColor);
	}

	void Renderer2D::DrawAnimationRotateQuad(const glm::vec3 & position, const glm::vec2 & size, float rotation, const Ref<Animation2D> animation, const glm::vec4 & tintColor)
	{
		GR_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_WithMask", 0);
		s_Data.TextureShader->SetFloat4("u_Color", tintColor);
		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		animation->Bind();

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f, })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", translate);

		s_Data.QuardVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuardVertexArray);
	}

	void Renderer2D::DrawAnimation(const glm::mat4 & translate, const Ref<Animation2D> animation, const glm::vec4 & tintColor)
	{
		GR_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_WithMask", 0);
		s_Data.TextureShader->SetFloat4("u_Color", tintColor);
		s_Data.TextureShader->SetMat4("u_Transform", translate);
		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);

		animation->Bind();
		s_Data.QuardVertexArray->Bind();

		RenderCommand::DrawIndexed(s_Data.QuardVertexArray);
	}

	void Renderer2D::DrawFixedQuad(const glm::mat4 & translate, const Ref<Texture>& texture, const glm::vec4 & tintColor)
	{
		GR_PROFILE_FUNCTION();

		s_Data.FixedShader->Bind();
		s_Data.FixedShader->SetMat4("u_Transform", translate);
		s_Data.FixedShader->SetFloat4("u_Color", tintColor);

		texture->Bind();
		
		s_Data.QuardVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuardVertexArray);
	}

	void Renderer2D::DrawFixedQuad(const glm::mat4 & translate, const Ref<FrameTexture2D>& frameTexture, int frameX, int frameY, const glm::vec4 & tintColor)
	{
		GR_PROFILE_FUNCTION();

		s_Data.FixedShader->Bind();
		//s_Data.FixedShader->SetInt("u_WithMask", 0);
		s_Data.FixedShader->SetMat4("u_Transform", translate);
		s_Data.FixedShader->SetFloat4("u_Color", tintColor);

		frameTexture->Bind(frameX, frameY);

		s_Data.QuardVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuardVertexArray);
	}

	void Renderer2D::DrawFixedAnimation(const glm::mat4& translate, const Ref<Animation2D>& animation, const glm::vec4& tintColor)
	{
		GR_PROFILE_FUNCTION();

		s_Data.FixedShader->Bind();
		//s_Data.FixedShader->SetInt("u_WithMask", 0);
		s_Data.FixedShader->SetFloat4("u_Color", tintColor);
		s_Data.FixedShader->SetMat4("u_Transform", translate);

		animation->Bind();

		s_Data.QuardVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuardVertexArray);
	}

	void Renderer2D::BatchDrawQuad(const glm::vec3 & position, const glm::vec2 & size, const glm::vec4 & color)
	{
		GR_PROFILE_FUNCTION();
		
		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::Flush()
	{
		s_Data.BatchShader->Bind(); 
		s_Data.QuardVertexBatchArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuardVertexBatchArray, s_Data.QuadIndexCount);
	}

	/*void Renderer2D::DrawFixedAnimationWithMask(const glm::mat4 & textureTranslate, const Ref<Animation2D>& animation, const glm::mat4 & maskTranslate, const Ref<Texture>& mask, const glm::vec4 & tintColor)
	{
		s_Data.FixedShader->Bind();
		s_Data.FixedShader->SetInt("u_WithMask", 1);
		s_Data.FixedShader->SetFloat4("u_Color", tintColor);
		s_Data.FixedShader->SetMat4("u_MaskCoord", maskTranslate);
		s_Data.FixedShader->SetMat4("u_Transform", textureTranslate);

		animation->Bind(0);
		mask->Bind(1);

		s_Data.QuardVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuardVertexArray);
	}*/

}