// Copyright (C) Explorer++ Project
// SPDX-License-Identifier: GPL-3.0-only
// See LICENSE in the top level directory

#pragma once

#include <memory>
#include <string>

struct Config;
class CoreInterface;
class DarkModeGroupBox;
class ResizableDialogHelper;

class OptionsPage
{
public:
	using SettingChangedCallback = std::function<void()>;

	OptionsPage(UINT dialogResourceId, UINT titleResourceId, HWND parent,
		HINSTANCE resourceInstance, Config *config, CoreInterface *coreInterface,
		SettingChangedCallback settingChangedCallback, HWND tooltipWindow);
	virtual ~OptionsPage();

	void InitializeDialog();
	HWND GetDialog() const;
	std::wstring GetTitle() const;

	virtual void SaveSettings() = 0;

protected:
	Config *const m_config;
	CoreInterface *const m_coreInterface;
	const HINSTANCE m_resourceInstance;
	const HWND m_tooltipWindow;

	std::unordered_set<int> m_checkboxControlIds;
	std::unordered_set<int> m_radioButtonControlIds;
	std::vector<std::unique_ptr<DarkModeGroupBox>> m_darkModeGroupBoxes;

	const SettingChangedCallback m_settingChangedCallback;

private:
	static INT_PTR CALLBACK DialogProcStub(HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam);
	INT_PTR CALLBACK DialogProc(HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam);
	INT_PTR OnPageCtlColorDlg(HWND hwnd, HDC hdc);
	INT_PTR OnCtlColor(HWND hwnd, HDC hdc);
	INT_PTR OnCustomDraw(const NMCUSTOMDRAW *customDraw);

	virtual std::unique_ptr<ResizableDialogHelper> InitializeResizeDialogHelper() = 0;
	virtual void InitializeControls() = 0;

	virtual void OnCommand(WPARAM wParam, LPARAM lParam);
	virtual INT_PTR OnNotify(WPARAM wParam, LPARAM lParam);

	HWND m_dialog = nullptr;
	const HWND m_parent;
	const UINT m_dialogResourceId;
	const UINT m_titleResourceId;
	std::unique_ptr<ResizableDialogHelper> m_resizableDialogHelper;
};
