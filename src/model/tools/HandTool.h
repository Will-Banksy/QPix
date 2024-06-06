#ifndef HANDTOOL_H
#define HANDTOOL_H

#include "AbstractTool.h"

class HandTool : public AbstractTool {
	Q_OBJECT

public:
	explicit HandTool();
	~HandTool() override;

	void onSelectedChanged(bool selected, AppModel* model) override;
};

#endif // HANDTOOL_H