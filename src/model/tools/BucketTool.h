#ifndef BUCKETTOOL_H
#define BUCKETTOOL_H

#include "AbstractTool.h"

class BucketTool : public AbstractTool {
	Q_OBJECT

public:
	explicit BucketTool();
	~BucketTool() override;

	void onClick(QImage& surface, QPoint pt, Qt::MouseButton button, AppModel* model) override;
};

#endif // BUCKETTOOL_H