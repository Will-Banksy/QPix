#ifndef MATRIX_H
#define MATRIX_H

/**
 * @todo write docs
 */
namespace utils {
	class BrushMatrix {
		public:
			BrushMatrix(ushort width, ushort height, QList<QPoint> points);
			~BrushMatrix();
			bool at(int i, int j);

		private:
			char* matrix;
			int width;
			int height;

			char* makePackedMatrix(ushort width, ushort height);
			void setAt(char* matrix, int i, int j, bool val);
	};
}

#endif // MATRIX_H
