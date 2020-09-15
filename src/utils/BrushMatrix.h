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
			bool at(int i, int j) const;
			void setAt(quint8* matrix, int i, int j, bool val);

			ushort width() const;
			ushort height() const;

		private:
			quint8* m_matrix; // Could actually use a QByteArray perhaps
			ushort m_width;
			ushort m_height;

			/// Actually makes an array
			quint8* makePackedMatrix(ushort width, ushort height);
	};
}

#endif // MATRIX_H
