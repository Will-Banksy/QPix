#include "BucketTool.h"
#include "utils/PaintUtils.h"
#include "model/AppModel.h"

BucketTool::BucketTool() : AbstractTool() {
	m_Name = "Fill Bucket";
	m_Description = "Fills a region";
	m_IconPath = ":/assets/tools/fill.svg";
	m_UsageType = ToolUsageType::Click;

	m_Settings = new ToolSettings({
		{ TS_BUCKET_DIAGONAL_FILL, ToolSettingInfo {
			.Title = "Fill Diagonally",
			.Description = "Floods the fill diagonally in addition to horizontally & vertically, allowing it to fill pixel-perfect lines",
			.Value = TSVariant::newBool(false)
		}}
	});
}

BucketTool::~BucketTool() {
	delete m_Settings;
}

void BucketTool::onClick(const QImage& surface, QImage& buffer, QPoint pt, Qt::MouseButton button, AppModel* model) {
	QRgb colour = button == Qt::MouseButton::RightButton ? model->secondaryColour().rgba() : model->primaryColour().rgba();

	bool fillDiagonally = m_Settings->get(TS_BUCKET_DIAGONAL_FILL).some().Value.toBool();

	PaintUtils::fillArea(buffer, pt.x(), pt.y(), colour, 0, fillDiagonally); // TODO: Tolerance
}

