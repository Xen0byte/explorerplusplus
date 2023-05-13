// Copyright (C) Explorer++ Project
// SPDX-License-Identifier: GPL-3.0-only
// See LICENSE in the top level directory

#pragma once

#include "OptionsPage.h"

class FilesFoldersOptionsPage : public OptionsPage
{
public:
	FilesFoldersOptionsPage(HWND parent, HINSTANCE resourceInstance, Config *config,
		CoreInterface *coreInterface, SettingChangedCallback settingChangedCallback,
		HWND tooltipWindow);

	void SaveSettings() override;

private:
	std::unique_ptr<ResizableDialogHelper> InitializeResizeDialogHelper() override;
	void InitializeControls() override;

	void OnCommand(WPARAM wParam, LPARAM lParam) override;

	void SetInfoTipControlStates();
	void SetFolderSizeControlState();
};
