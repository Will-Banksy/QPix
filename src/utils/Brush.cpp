#include "Brush.h"

utils::CustomBrush::CustomBrush(ushort width, ushort height, QList<QPoint> points) : DefaultBrush(), brushMatrix(BrushMatrix(width, height, points)) {
}