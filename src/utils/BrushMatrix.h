#ifndef MATRIX_H
#define MATRIX_H

#include <QPoint>
#include <QList>

/**
 * @todo write docs
 */
namespace utils {
	class BrushMatrix {
		public:
			BrushMatrix();
			BrushMatrix(ushort width, ushort height, const QList<QPoint>& points);
			~BrushMatrix();
			bool at(int i, int j);

		private:
			uchar* matrix;
			ushort width;
			ushort height;

			uchar* makePackedMatrix(ushort width, ushort height);
			void setAt(uchar* matrix, int i, int j, bool val);
	};
}

#endif // MATRIX_H
