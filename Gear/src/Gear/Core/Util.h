#pragma once

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

			float Left, Top, Right, Bottom;
			float CenterX, CenterY;
			float Width, Height;
		};

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
	}
}