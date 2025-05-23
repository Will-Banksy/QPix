#ifndef BUCKETTOOL_H
#define BUCKETTOOL_H

#include "AbstractTool.h"

const static QString TS_BUCKET_DIAGONAL_FILL = QString("bucket.diagonal-fill");

class BucketTool : public AbstractTool {
	Q_OBJECT

public:
	explicit BucketTool();
	~BucketTool() override;

	void onClick(const QImage& surface, QImage& buffer, QPoint pt, Qt::MouseButton button, AppModel* model) override;
};

#endif // BUCKETTOOL_H