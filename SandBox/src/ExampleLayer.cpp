#include "Gear.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui/imgui.h"

class ExampleLayer
	: public Gear::Layer {
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f)
	{
		/*//triangle
		//1. Create VertexArray
		m_VertexArray = Gear::VertexArray::Create();
		//2. define vertices, Indices
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f,  -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			0.0f,   0.5f, 0.0f, 0.8f, 0.7f, 0.2f, 1.0f
		};
		unsigned int indices[3] = {
			0, 1, 2
		};
		//3. Create VertexBuffer
		Gear::Ref<Gear::VertexBuffer> vertexBuffer;
		vertexBuffer = Gear::VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->SetLayout({
			{Gear::ShaderDataType::Float3, "a_Position"},
			{Gear::ShaderDataType::Float4, "a_Color"},
			});
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		//4. Create IndexBuffer
		Gear::Ref<Gear::IndexBuffer> indexBuffer;
		indexBuffer = Gear::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);
		//5. define shader
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;			
			layout(location = 1) in vec4 a_Color;			
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;			
			out vec4 v_Color;			

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";
		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;			
			
			in vec3 v_Position;			
			in vec4 v_Color;	

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";
		m_Shader = Gear::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		//Square
		//1. Create VertexArray
		m_SqaureVA = Gear::VertexArray::Create();
		//2. define vertices, Indices
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		unsigned int squreIndices[6] = {
			0, 1, 2, 2, 3, 0
		};
		//3. Create VertexBuffer
		Gear::Ref<Gear::VertexBuffer> squareVB;
		squareVB = Gear::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{Gear::ShaderDataType::Float3, "a_Position"},
			{Gear::ShaderDataType::Float2, "a_TexCoord"}
			});
		m_SqaureVA->AddVertexBuffer(squareVB);
		//4. Create IndexBuffer
		Gear::Ref<Gear::IndexBuffer> squreIB;
		squreIB = Gear::IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t));
		m_SqaureVA->SetIndexBuffer(squreIB);
		//5. define shader
		std::string flatColorShadervertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;			
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;			

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";
		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;			
			
			in vec3 v_Position;			
			
			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";
		m_flatColorShader = Gear::Shader::Create("FlatColor", flatColorShadervertexSrc, flatColorShaderFragmentSrc);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Gear::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Gear::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Gear::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Gear::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);*/
	}

	void OnUpdate(Gear::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);

		Gear::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Gear::RenderCommand::Clear();

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Gear::OpenGLShader>(m_flatColorShader)->Bind();
		std::dynamic_pointer_cast<Gear::OpenGLShader>(m_flatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		Gear::Renderer::BeginScene(m_CameraController.GetCamera());
		for (int y = 0; y < 20; ++y)
		{
			for (int x = 0; x < 20; ++x)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Gear::Renderer::Submit(m_flatColorShader, m_SqaureVA, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Gear::Renderer::Submit(textureShader, m_SqaureVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_ChernoLogoTexture->Bind();
		Gear::Renderer::Submit(textureShader, m_SqaureVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		//triangle
		//Gear::Renderer::Submit(m_Shader, m_VertexArray);

		Gear::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Sqaure Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Gear::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

private:
	Gear::ShaderLibrary m_ShaderLibrary;
	Gear::Ref<Gear::Shader> m_Shader;
	Gear::Ref<Gear::VertexArray> m_VertexArray;

	Gear::Ref<Gear::Shader> m_flatColorShader;
	Gear::Ref<Gear::VertexArray> m_SqaureVA;

	Gear::Ref<Gear::Texture2D> m_Texture, m_ChernoLogoTexture;

	Gear::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};