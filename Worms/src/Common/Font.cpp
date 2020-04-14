#include "wmpch.h"
#include "Font.h"


std::vector<Gear::Ref<Gear::FrameTexture2D>> Font::m_Fonts = std::vector<Gear::Ref<Gear::FrameTexture2D>>();
std::unordered_map<char, int> Font::m_Indexer = std::unordered_map<char, int>();
glm::vec3 Font::m_Scale = glm::vec3(0.0f);

void Font::Init()
{
	m_Fonts.resize(FontType::FontEnd);

	m_Fonts[FontType::Red] = Gear::FrameTexture2D::Create("assets/textures/Font/Red.png", 1, 160);
	m_Fonts[FontType::RedSmall] = Gear::FrameTexture2D::Create("assets/textures/Font/RedSmall.png", 1, 160);
	m_Fonts[FontType::Blue] = Gear::FrameTexture2D::Create("assets/textures/Font/Blue.png", 1, 160);
	m_Fonts[FontType::BlueSmall] = Gear::FrameTexture2D::Create("assets/textures/Font/BlueSmall.png", 1, 160);
	m_Fonts[FontType::White] = Gear::FrameTexture2D::Create("assets/textures/Font/White.png", 1, 160);
	m_Fonts[FontType::WhiteSmall] = Gear::FrameTexture2D::Create("assets/textures/Font/WhiteSmall.png", 1, 160);
	m_Fonts[FontType::GraySmall] = Gear::FrameTexture2D::Create("assets/textures/Font/GraySmall.png", 1, 160);
	m_Fonts[FontType::RedNumber] = Gear::FrameTexture2D::Create("assets/textures/Font/RedNumber.png", 1, 10);
	m_Fonts[FontType::WhiteNumber] = Gear::FrameTexture2D::Create("assets/textures/Font/WhiteNumber.png", 1, 10);
	m_Fonts[FontType::GrayNumber] = Gear::FrameTexture2D::Create("assets/textures/Font/GrayNumber.png", 1, 10);

	for (int i = 0; i < 26; ++i)
	{
		m_Indexer['A' + i] = i;
	}
	for (int i = 0; i < 26; ++i)
	{
		m_Indexer['a' + i] = i + 26;
	}
	for (int i = 0; i < 10; ++i)
	{
		m_Indexer['0' + i] = i + 52;
	}
	m_Indexer['`'] = 62; m_Indexer['!'] = 63; m_Indexer['@'] = 64;
	m_Indexer['#'] = 65; m_Indexer['$'] = 66; m_Indexer['%'] = 67;
	m_Indexer['^'] = 68; m_Indexer['&'] = 69; m_Indexer['*'] = 70;
	m_Indexer['('] = 71; m_Indexer[')'] = 72; m_Indexer['-'] = 73;
	m_Indexer['_'] = 74; m_Indexer['='] = 75; m_Indexer['+'] = 76;
	m_Indexer['['] = 77; m_Indexer['{'] = 78; m_Indexer[']'] = 79;
	m_Indexer['}'] = 80; m_Indexer[';'] = 81; m_Indexer[':'] = 82;
	m_Indexer['\''] = 83; m_Indexer['"'] = 84; m_Indexer['\\'] = 85;
	m_Indexer['|'] = 86; m_Indexer[','] = 87; m_Indexer['<'] = 88;
	m_Indexer['.'] = 89; m_Indexer['>'] = 90; m_Indexer['/'] = 91;
	m_Indexer['?'] = 92; m_Indexer['~'] = 94;

}

void Font::ShoutDown()
{
	m_Fonts.clear();
	m_Indexer.clear();
}

void Font::PrintFont(const glm::vec3 & midPosition, const glm::vec3& scale, const std::string & str, FontType::Type font, float offset, bool fixed, const glm::vec4& tintColor)
{
	int len = str.length();
	float spaceOfLetter = -(len * 0.5) * offset;
	if (font == FontType::RedNumber || font == FontType::WhiteNumber || font == FontType::GrayNumber)
	{
		for (int i = 0; i < len; ++i)
		{
			if (i >= 1)
			{
				spaceOfLetter += offset;
			}
			glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(midPosition.x + spaceOfLetter, midPosition.y, midPosition.z))
				* glm::scale(glm::mat4(1.0f), scale);
			if (fixed)
			{
				Gear::Renderer2D::DrawFixedQuad(trans, m_Fonts[font], 0, 9 - (str[i] - '0'));
			}
			else
			{
				Gear::Renderer2D::DrawFrameQuad(trans, m_Fonts[font], 0, 9 - (str[i] - '0'), tintColor);
			}
		}
	}
	else
	{
		for (int i = 0; i < len; ++i)
		{
			if (i >= 1)
			{
				if (str[i] == ' ')
				{
					spaceOfLetter += offset * 0.5f;
					continue;
				}
				else
				{
					spaceOfLetter += offset;
				}
			}
			glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(midPosition.x + spaceOfLetter, midPosition.y, midPosition.z))
				* glm::scale(glm::mat4(1.0f), scale);
			
			if (fixed)
			{
				Gear::Renderer2D::DrawFixedQuad(trans, m_Fonts[font], 0, 159 - m_Indexer[str[i]]);
			}
			else
			{
				Gear::Renderer2D::DrawFrameQuad(trans, m_Fonts[font], 0, 159 - m_Indexer[str[i]]);
			}
		}
	}
}

