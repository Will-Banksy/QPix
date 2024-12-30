#ifndef PIPETTETOOL_H
#define PIPETTETOOL_H

#include "AbstractTool.h"

const static QString TS_PIPETTE_COPYALPHA = QString("Copy Alpha");

class PipetteTool : public AbstractTool {
	Q_OBJECT

public:
	explicit PipetteTool();
	~PipetteTool() override;

    void onClick(const QImage& surface, QImage& buffer, QPoint pt, Qt::MouseButton button, AppModel* model) override;
};

#endif // PIPETTETOOL_H