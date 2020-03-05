#include "grpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Gear {

	static GLenum ShaderDataTypeToOpenGlBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case Gear::ShaderDataType::Float:	return GL_FLOAT;
			case Gear::ShaderDataType::Float2:	return GL_FLOAT;
			case Gear::ShaderDataType::Float3:	return GL_FLOAT;
			case Gear::ShaderDataType::Float4:	return GL_FLOAT;
			case Gear::ShaderDataType::Mat3:	return GL_FLOAT;
			case Gear::ShaderDataType::Mat4:	return GL_FLOAT;
			case Gear::ShaderDataType::Int:	return GL_INT;
			case Gear::ShaderDataType::Int2:	return GL_INT;
			case Gear::ShaderDataType::Int3:	return GL_INT;
			case Gear::ShaderDataType::Int4:	return GL_INT;
			case Gear::ShaderDataType::Bool:	return GL_BOOL;
		}

		GR_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		GR_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		GR_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind() const
	{
		GR_PROFILE_FUNCTION();

		glBindVertexArray(0);

	}
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		GR_PROFILE_FUNCTION();

		GR_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
		
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGlBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		GR_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}