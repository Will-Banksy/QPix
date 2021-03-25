#ifndef COLOUR_H
#define COLOUR_H

#include <cstdint>

namespace utils {
	/// This class deals with colours as in ARGB
    class Colour { // TODO: Provide a definition for each method
	    public:
		    static uint32_t TRANSPARENT;
			static uint32_t BLACK;
			static uint32_t WHITE;
			static uint32_t RED;
			static uint32_t GREEN;
			static uint32_t BLUE;
			static uint32_t CYAN;
			static uint32_t MAGENTA;
			static uint32_t YELLOW;

			static uint32_t toIntARGB(uint32_t alpha, uint32_t red, uint32_t green, uint32_t blue);
			static uint32_t toIntACMYK(uint32_t alpha, uint32_t cyan, uint32_t magenta, uint32_t yellow, uint32_t key);
			static uint32_t toIntAHSB(uint32_t alpha, uint32_t hue, uint32_t saturation, uint32_t brightness);
			static uint32_t toIntAHSL(uint32_t apha, uint32_t hue, uint32_t saturation, uint32_t lightness);

			static uint32_t RGBtoCMYK(uint32_t colour, uint32_t& cyan, uint32_t& magenta, uint32_t& yellow);
			static uint32_t RGBtoHSB(uint32_t colour, uint32_t& hue, uint32_t& saturation, uint32_t& brightness);
			static uint32_t RGBtoHSL(uint32_t colour, uint32_t& hue, uint32_t& saturation, uint32_t& lightness);

            static inline uint32_t getAlpha(uint32_t colour);
            static inline uint32_t getRed(uint32_t colour);
            static inline uint32_t getGreen(uint32_t colour);
            static inline uint32_t getBlue(uint32_t colour);
			static uint32_t getCyan(uint32_t colour);
			static uint32_t getMagenta(uint32_t colour);
			static uint32_t getYellow(uint32_t colour);
			static uint32_t getHue(uint32_t colour);
			static uint32_t getHSBSaturation(uint32_t colour);
			static uint32_t getBrightness(uint32_t colour);
			static uint32_t getHSLSaturation(uint32_t colour);
			static uint32_t getLightness(uint32_t colour);

			static uint16_t getDifference(uint32_t colour1, uint32_t colour2, bool useAlpha = false);

			static void setAlpha(uint32_t& colour);
			static void setRed(uint32_t& colour);
			static void setGreen(uint32_t& colour);
			static void setBlue(uint32_t& colour);

	    private:
			Colour() = delete;
	};

	// Define the inline functions, as they neccessarily require one single definition

	uint32_t Colour::getAlpha(uint32_t colour) {
		return (colour >> 24) & 0xFF;
	}

	uint32_t Colour::getRed(uint32_t colour) {
		return (colour >> 16) & 0xFF;
	}

	uint32_t Colour::getGreen(uint32_t colour) {
		return (colour >> 8) & 0xFF;
	}

	uint32_t Colour::getBlue(uint32_t colour) {
		return colour & 0xFF;
	}
}

#endif // COLOUR_H
