#include "Colour.h"
#include <cmath>

namespace utils {
	// Declare inline functions --
	static inline uint32_t constrain(uint32_t val, uint32_t min, uint32_t max) {
		return val < min ? min : (val > max ? max : val);
	};

	static inline float getMin(float val1, float val2, float val3) {
		if(val1 < val2) {
			if(val1 < val3)
				return val1;
			else
				return val3;
		} else {
			if(val2 < val3)
				return val2;
			else
				return val3;
		}
	}

	static inline float getMax(float val1, float val2) {
		if(val1 > val2)
			return val1;
		else
			return val2;
	}

	static inline float getMin(float val1, float val2) {
		if(val1 < val2)
			return val1;
		else
			return val2;
	}

	static inline int sq(int val) {
		return val * val;
	}
	// --

	uint32_t Colour::TRANSPARENT = (uint32_t)0x00000000;
	uint32_t Colour::BLACK = 0xff000000;
	uint32_t Colour::WHITE = 0xffffffff;
	uint32_t Colour::RED   = 0xffff0000;
	uint32_t Colour::GREEN = 0xff00ff00;
	uint32_t Colour::BLUE  = 0xff0000ff;
	uint32_t Colour::CYAN  = 0xff00ffff;
	uint32_t Colour::MAGENTA = 0xffff00ff;
	uint32_t Colour::YELLOW  = 0xffffff00;

	uint32_t Colour::toIntARGB(uint32_t alpha, uint32_t red, uint32_t green, uint32_t blue) {
		return (alpha << 24) + (red << 16) + (green << 8) + blue;
	}

	uint32_t Colour::toIntACMYK(uint32_t alpha, uint32_t cyan, uint32_t magenta, uint32_t yellow, uint32_t key) {
		float c = (float)cyan;
		float m = (float)magenta;
		float y = (float)yellow;
		float k = (float)key;

		float r1 = round(255 * (1 - c / 255) * (1 - k / 255));
		float g1 = round(255 * (1 - m / 255) * (1 - k / 255));
		float b1 = round(255 * (1 - y / 255) * (1 - k / 255));

		uint32_t r = constrain((uint32_t)r1, 0, 255);
		uint32_t g = constrain((uint32_t)g1, 0, 255);
		uint32_t b = constrain((uint32_t)b1, 0, 255);

		return toIntARGB(alpha, r, g, b);
	}

	uint32_t Colour::toIntAHSB(uint32_t alpha, uint32_t hue, uint32_t saturation, uint32_t brightness) {
		float H = (float)hue;
		float S = (float)saturation / 100;
		float Br = (float)brightness / 100;

		float k = fmod((5.0f) + H / 60.0f, 6.0f);
		float R = Br - Br * S * getMax(0, getMin(k, 4 - k, 1));

		k = fmod((3.0f) + H / 60.0f, 6.0f);
		float G = Br - Br * S * getMax(0, getMin(k, 4 - k, 1));

		k = fmod((1.0f) + H / 60.0f, 6.0f);
		float B = Br - Br * S * getMax(0, getMin(k, 4 - k, 1));

		int red = (int)constrain(round(R * 255), 0, 255);
		int green = (int)constrain(round(G * 255), 0, 255);
		int blue = (int)constrain(round(B * 255), 0, 255);

		return toIntARGB(alpha, red, green, blue);
	}

	uint32_t Colour::toIntAHSL(uint32_t alpha, uint32_t hue, uint32_t saturation, uint32_t lightness) {
		float H = (float)hue;
		float S = (float)saturation / 100;
		float L = (float)lightness / 100;

		float a = S * getMin(L, 1 - L);

		float k = fmod(0 + H / 30, 12);
		float R1 = L - a * getMax(getMin(k - 3, 9 - k, 1), -1);

		k = fmod(8 + H / 30, 12);
		float G1 = L - a * getMax(getMin(k - 3, 9 - k, 1), -1);

		k = fmod(4 + H / 30, 12);
		float B1 = L - a * getMax(getMin(k - 3, 9 - k, 1), -1);

		int R = (int)constrain(round(R1 * 255), 0, 255);
		int G = (int)constrain(round(G1 * 255), 0, 255);
		int B = (int)constrain(round(B1 * 255), 0, 255);

		return toIntARGB(alpha, R, G, B);
	}

	uint16_t Colour::getDifference(uint32_t colour1, uint32_t colour2, bool useAlpha) {
		int diffA = sq(getAlpha(colour2) - getAlpha(colour1));
		int diffR = sq(getRed(colour2) - getRed(colour1));
		int diffG = sq(getGreen(colour2) - getGreen(colour1));
		int diffB = sq(getBlue(colour2) - getBlue(colour1));
		if(useAlpha) {
			// Max difference: 510 (which is 255 * 2)
			return (uint16_t)round(sqrt(diffA + diffR + diffG + diffB));
		} else {
			// Max difference: 441.673 => 442
			return (uint16_t)round(sqrt(diffR + diffG + diffB));
		}
	}
}