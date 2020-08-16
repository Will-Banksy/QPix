#ifndef COLOUR_H
#define COLOUR_H

#include <stdlib.h>

namespace utils {
	/// This class deals with colours as in ARGB
    class Colour {
	    public:
		    static uint TRANSPARENT;
			static uint BLACK;
			static uint WHITE;
			static uint RED;
			static uint GREEN;
			static uint BLUE;
			static uint CYAN;
			static uint MAGENTA;
			static uint YELLOW;

			static uint toIntARGB(uint alpha, uint red, uint green, uint blue);
			static uint toIntACMYK(uint alpha, uint cyan, uint magenta, uint yellow, uint key);
			static uint toIntAHSB(uint alpha, uint hue, uint saturation, uint brightness);
			static uint toIntAHSL(uint apha, uint hue, uint saturation, uint lightness);

			static uint RGBtoCMYK(uint colour, uint& cyan, uint& magenta, uint& yellow);
			static uint RGBtoHSB(uint colour, uint& hue, uint& saturation, uint& brightness);
			static uint RGBtoHSL(uint colour, uint& hue, uint& saturation, uint& lightness);

            static inline uint getAlpha(uint colour);
            static inline uint getRed(uint colour);
            static inline uint getGreen(uint colour);
            static inline uint getBlue(uint colour);
			static uint getCyan(uint colour);
			static uint getMagenta(uint colour);
			static uint getYellow(uint colour);
			static uint getHue(uint colour);
			static uint getHSBSaturation(uint colour);
			static uint getBrightness(uint colour);
			static uint getHSLSaturation(uint colour);
			static uint getLightness(uint colour);

			static ushort getDifference(uint colour1, uint colour2, bool useAlpha = false);

			static void setAlpha(uint& colour);
			static void setRed(uint& colour);
			static void setGreen(uint& colour);
			static void setBlue(uint& colour);

	    private:
			Colour();
	};

	// Define the inline functions, as they neccessarily require one single definition

	uint Colour::getAlpha(uint colour) {
		return (colour >> 24) & 0xFF;
	}

	uint Colour::getRed(uint colour) {
		return (colour >> 16) & 0xFF;
	}

	uint Colour::getGreen(uint colour) {
		return (colour >> 8) & 0xFF;
	}

	uint Colour::getBlue(uint colour) {
		return colour & 0xFF;
	}
}

#endif // COLOUR_H
