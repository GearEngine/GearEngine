#pragma once

#include <glm/glm.hpp>

namespace Gear {

	namespace Util {

		struct FRect
		{
			FRect()
				: Left(0.0f), Top(0.0f), Right(0.0f), Bottom(0.0f),
				Width(0.0f), Height(0.0f),
				CenterX(0.0f), CenterY(0.0f)
			{}

			FRect(float left, float top, float right, float bottom)
				: Left(left), Top(top), Right(right), Bottom(bottom)
			{
				Width = Right - Left;
				Height = Top - Bottom;
				CenterX = Left + Width / 2;
				CenterY = Bottom + Height / 2;
			}


			void ResetPos(float centerX, float centerY, float halfWidth, float halfHeight)
			{
				CenterX = centerX;
				CenterY = centerY;

				Left = CenterX - halfWidth;
				Right = CenterX + halfWidth;
				Top = CenterY + halfHeight;
				Bottom = CenterY - halfHeight;
			}

			void Set(float centerX, float centerY, float halfWidth, float halfHeight)
			{
				CenterX = centerX;
				CenterY = centerY;

				Left = CenterX - halfWidth;
				Right = CenterX + halfWidth;
				Top = CenterY + halfHeight;
				Bottom = CenterY - halfHeight;
			}

			void Set(const glm::mat4& transform)
			{
				Set(transform[3][0], transform[3][1], transform[0][0] / 2, transform[1][1] / 2);
			}

			float Left, Top, Right, Bottom;
			float CenterX, CenterY;
			float Width, Height;
		};

		inline bool IsPointRectCollision(const std::pair<float, float>& point, const FRect& rect)
		{
			if ((rect.Left < point.first && point.first < rect.Right) &&
				(rect.Bottom < point.second && point.second < rect.Top))
				return true;
			return false;
		}

		inline float GetRndFloat(float zeroToNum)
		{
			return rand() / (float)RAND_MAX * zeroToNum;
		}

		inline float GetRndFloatFromTo(float fromNum, float toNum)
		{
			if (fromNum > toNum)
				std::swap(toNum, fromNum);
			return rand() / (float)RAND_MAX * (toNum - fromNum) + fromNum;
		}

		inline int GetRndInt(int zeroToNum)
		{
			return rand() % zeroToNum;
		}

		inline int GetRndIntFromTo(int fromNum, int toNum)
		{
			if (fromNum > toNum)
				std::swap(toNum, fromNum);
			int ret = toNum - fromNum;
			return rand() % ret + fromNum;
		}

		float GetAngleFromXY(float x, float y);

		inline std::vector<std::string> GetStrTokken(const std::string& path, const char delimiter)
		{
			std::vector<std::string> ret;
			std::string token;
			std::stringstream ss(path);

			while (getline(ss, token, delimiter))
			{
				ret.push_back(token);
			}
			return ret;
		}
	}
}