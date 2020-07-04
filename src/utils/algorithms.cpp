#include "algorithms.h"

inline int abs(int val) {
	if(val < 0) {
		return val * -1;
	}
	return val;
}

void Algorithms::plotLine(int startX, int startY, int endX, int endY, uint col, Tool::ToolSettings& settings, AlgoAction& action) {
	int& x0 = startX;
	int& y0 = startY;
	int& x1 = endX;
	int& y1 = endY;

	int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
	int err = dx+dy, e2; /* error value e_xy */

	for(;;){  /* loop */
		// Call the function with the () operator
		action(x0, y0, col, settings);
		if (x0==x1 && y0==y1) break;
		e2 = 2*err;
		if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
			if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
	}
}