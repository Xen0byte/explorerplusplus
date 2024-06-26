// Copyright (C) Explorer++ Project
// SPDX-License-Identifier: GPL-3.0-only
// See LICENSE in the top level directory

#pragma once

#include "BetterEnumsWrapper.h"
#include <windows.h>
#include <functional>
#include <list>
#include <string>
#include <type_traits>

namespace RegistrySettings
{

LSTATUS SaveDword(HKEY key, const std::wstring &valueName, DWORD value);
LSTATUS ReadDword(HKEY key, const std::wstring &subKey, const std::wstring &valueName,
	DWORD &output);
LSTATUS ReadDword(HKEY key, const std::wstring &valueName, DWORD &output);
void ReadDword(HKEY key, const std::wstring &valueName,
	std::function<void(DWORD value)> successCallback);
LSTATUS SaveString(HKEY key, const std::wstring &valueName, const std::wstring &value);
LSTATUS ReadString(HKEY key, const std::wstring &valueName, std::wstring &output);
LSTATUS SaveStringList(HKEY key, const std::wstring &baseValueName,
	const std::list<std::wstring> &strings);
LSTATUS ReadStringList(HKEY key, const std::wstring &baseValueName,
	std::list<std::wstring> &outputStrings);
bool SaveDateTime(HKEY key, const std::wstring &baseValueName, const FILETIME &dateTime);
bool ReadDateTime(HKEY key, const std::wstring &baseValueName, FILETIME &outputDateTime);
LSTATUS SaveBinaryValue(HKEY key, const std::wstring &valueName, const BYTE *data, DWORD length);
LSTATUS ReadBinaryValueSize(HKEY key, const std::wstring &valueName, DWORD &length);
LSTATUS ReadBinaryValue(HKEY key, const std::wstring &valueName, void *data, DWORD length);

template <typename T>
	requires std::is_trivially_copyable_v<T> && std::is_trivially_constructible_v<T>
LSTATUS SaveVectorToBinaryValue(HKEY key, const std::wstring &valueName,
	const std::vector<T> &items)
{
	return RegistrySettings::SaveBinaryValue(key, valueName,
		reinterpret_cast<const BYTE *>(items.data()), static_cast<DWORD>(items.size() * sizeof(T)));
}

template <typename T>
	requires std::is_trivially_copyable_v<T> && std::is_trivially_constructible_v<T>
LSTATUS ReadVectorFromBinaryValue(HKEY key, const std::wstring &valueName, std::vector<T> &output)
{
	DWORD length = 0;
	auto res = RegistrySettings::ReadBinaryValueSize(key, valueName, length);

	if (res != ERROR_SUCCESS)
	{
		return res;
	}

	if ((length % sizeof(T)) != 0)
	{
		return ERROR_BAD_LENGTH;
	}

	std::vector<T> items(length / sizeof(T));
	res = RegistrySettings::ReadBinaryValue(key, valueName, items.data(),
		static_cast<DWORD>(items.size() * sizeof(T)));

	if (res != ERROR_SUCCESS)
	{
		return res;
	}

	output = items;

	return ERROR_SUCCESS;
}

template <typename T>
LSTATUS Read32BitValueFromRegistry(HKEY key, const std::wstring &valueName, T &output)
{
	DWORD value;
	auto res = ReadDword(key, valueName, value);

	if (res == ERROR_SUCCESS)
	{
		output = value;
	}

	return res;
}

template <BetterEnum T>
void ReadBetterEnumValue(HKEY key, const std::wstring &valueName, T &output)
{
	DWORD value;
	auto res = ReadDword(key, valueName, value);

	if (res != ERROR_SUCCESS || !T::_is_valid(value))
	{
		return;
	}

	output = T::_from_integral(value);
}

}
