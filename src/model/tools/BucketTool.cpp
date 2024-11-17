#include "BucketTool.h"
#include "utils/PaintUtils.h"
#include "model/AppModel.h"

BucketTool::BucketTool() : AbstractTool() {
	m_Name = "Fill Bucket";
	m_Description = "Fills a region";
	m_IconPath = ":/data/tools/fill.png";
	m_UsageType = ToolUsageType::Click;

	m_Settings = new ToolSettings({
		{ TS_BUCKET_DIAGONAL_FILL, QVariant(false) }
	});

	qDebug() << "bucket settings len: " << m_Settings->getMap().size();
}

BucketTool::~BucketTool() {
	delete m_Settings;
}

void BucketTool::onClick(QImage& surface, QPoint pt, Qt::MouseButton button, AppModel* model) {
	QRgb colour = button == Qt::MouseButton::RightButton ? model->secondaryColour().rgba() : model->primaryColour().rgba();

	bool fillDiagonally = m_Settings->get(TS_BUCKET_DIAGONAL_FILL).unwrap()->toBool();

	PaintUtils::fillArea(surface, pt.x(), pt.y(), colour, 0, fillDiagonally); // TODO: Tolerance
}

