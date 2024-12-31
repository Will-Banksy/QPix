#include "BucketTool.h"
#include "utils/PaintUtils.h"
#include "model/AppModel.h"

BucketTool::BucketTool() : AbstractTool() {
	m_Name = "Fill Bucket";
	m_Description = "Fills a region";
	m_IconPath = ":/data/tools/fill.svg";
	m_UsageType = ToolUsageType::Click;

	m_Settings = new ToolSettings({
		{ TS_BUCKET_DIAGONAL_FILL, QVariant(false) }
	});
}

BucketTool::~BucketTool() {
	delete m_Settings;
}

void BucketTool::onClick(const QImage& surface, QImage& buffer, QPoint pt, Qt::MouseButton button, AppModel* model) {
	QRgb colour = button == Qt::MouseButton::RightButton ? model->secondaryColour().rgba() : model->primaryColour().rgba();

	bool fillDiagonally = m_Settings->get(TS_BUCKET_DIAGONAL_FILL).unwrap()->toBool();

	PaintUtils::fillArea(buffer, pt.x(), pt.y(), colour, 0, fillDiagonally); // TODO: Tolerance
}

