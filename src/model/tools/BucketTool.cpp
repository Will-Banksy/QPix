#include "BucketTool.h"
#include "utils/PaintUtils.h"
#include "model/AppModel.h"

BucketTool::BucketTool() : AbstractTool() {
	m_Name = "Fill Bucket";
	m_Description = "Fills a region";
	m_IconPath = ":/tools/fill.png";
	m_UsageType = ToolUsageType::Click;
}

BucketTool::~BucketTool() {
}

void BucketTool::onClick(QImage& surface, QPoint pt, Qt::MouseButton button, AppModel* model) {
	QRgb colour = button == Qt::MouseButton::RightButton ? model->secondaryColour().rgba() : model->primaryColour().rgba();

	PaintUtils::fillArea(surface, pt.x(), pt.y(), colour, 0); // TODO: Tolerance
}

