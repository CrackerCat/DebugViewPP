// (C) Copyright Gert-Jan de Vos and Jan Wilmans 2013.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

// Repository at: https://github.com/djeedjay/DebugViewPP/

#include "stdafx.h"
#include "Win32Lib/utilities.h"
#include "Resource.h"
#include "SourcesPage.h"

namespace fusion {
namespace debugviewpp {

CSourcesPageImpl::CSourcesPageImpl()
{
}

BEGIN_MSG_MAP_TRY(CSourcesPageImpl)
	MSG_WM_INITDIALOG(OnInitDialog)
	MSG_WM_DESTROY(OnDestroy)
	NOTIFY_CODE_HANDLER_EX(PIN_ADDITEM, OnAddItem)
	NOTIFY_CODE_HANDLER_EX(PIN_CLICK, OnClickItem)
	NOTIFY_CODE_HANDLER_EX(PIN_ITEMCHANGED, OnItemChanged)
	REFLECT_NOTIFICATIONS()
	CHAIN_MSG_MAP(CDialogResize<CSourcesPageImpl>)
END_MSG_MAP_CATCH(ExceptionHandler)

void CSourcesPageImpl::ExceptionHandler()
{
	MessageBox(WStr(GetExceptionMessage()), LoadString(IDR_APPNAME).c_str(), MB_ICONERROR | MB_OK);
}

void CSourcesPageImpl::AddFilter(const Filter& /*filter*/)
{
	//auto pFilterProp = PropCreateSimple(L"", WStr(filter.text));
	//pFilterProp->SetBkColor(filter.bgColor);
	//pFilterProp->SetTextColor(filter.fgColor);

	//auto pBkColor = PropCreateColorItem(L"Background Color", filter.bgColor);
	//pBkColor->SetEnabled(filter.filterType != FilterType::Exclude);

	//auto pTxColor = PropCreateColorItem(L"Text Color", filter.fgColor);
	//pTxColor->SetEnabled(filter.filterType != FilterType::Exclude);

	//int item = m_grid.GetItemCount();
	//m_grid.InsertItem(item, PropCreateCheckButton(L"", filter.enable));
	//m_grid.SetSubItem(item, 1, pFilterProp);
	//m_grid.SetSubItem(item, 2, CreateEnumTypeItem(L"", m_matchTypes, m_matchTypeCount, filter.matchType));
	//m_grid.SetSubItem(item, 3, CreateEnumTypeItem(L"", m_filterTypes, m_filterTypeCount, filter.filterType));
	//m_grid.SetSubItem(item, 4, pBkColor);
	//m_grid.SetSubItem(item, 5, pTxColor);
	//m_grid.SetSubItem(item, 6, PropCreateReadOnlyItem(L"", L"�"));
	//m_grid.SelectItem(item);
}

BOOL CSourcesPageImpl::OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/)
{
	m_grid.SubclassWindow(GetDlgItem(IDC_SOURCES_GRID));
	m_grid.InsertColumn(0, L"", LVCFMT_LEFT, 32, 0);
	m_grid.InsertColumn(1, L"Source", LVCFMT_LEFT, 336, 0);
	m_grid.InsertColumn(2, L"Type", LVCFMT_LEFT, 60, 0);
	m_grid.InsertColumn(3, L"", LVCFMT_LEFT, 16, 0);
	m_grid.SetExtendedGridStyle(PGS_EX_SINGLECLICKEDIT);


	for (auto it=m_logsources.begin(); it != m_logsources.end(); ++it)
	{
		auto description = (*it)->GetDescription();
		auto type = (*it)->GetSourceType();
		auto typeName = WStr(SourceTypeToString(type));

		int item = m_grid.GetItemCount();
		m_grid.InsertItem(item, PropCreateCheckButton(L"", true));
		m_grid.SetSubItem(item, 1, PropCreateReadOnlyItem(L"", description.c_str()));
		m_grid.SetSubItem(item, 2, PropCreateReadOnlyItem(L"", typeName.c_str()));
		if (type == SourceType::System)
			m_grid.SetSubItem(item, 3, PropCreateReadOnlyItem(L"", L""));
		else
			m_grid.SetSubItem(item, 3, PropCreateReadOnlyItem(L"", L"�"));
	}

	//UpdateGrid();
	DlgResize_Init(false);
	return TRUE;
}

void CSourcesPageImpl::OnDestroy()
{
}

LRESULT CSourcesPageImpl::OnAddItem(NMHDR* /*pnmh*/)
{
	//AddFilter(Filter());
	return 0;
}

LRESULT CSourcesPageImpl::OnClickItem(NMHDR* pnmh)
{
	auto& nmhdr = *reinterpret_cast<NMPROPERTYITEM*>(pnmh);

	int iItem = -1;
	int iSubItem = -1;
	if (m_grid.FindProperty(nmhdr.prop, iItem, iSubItem) && iSubItem == 3)
	{
		if (GetSourceType(iItem) != SourceType::System)
		{
			m_grid.DeleteItem(iItem);
			auto source = m_logsources[iItem];
			m_deleteSources.push_back(source);
			m_logsources.erase(m_logsources.begin()+iItem);
			return TRUE;
		}
	}
	return FALSE;
}

LRESULT CSourcesPageImpl::OnItemChanged(NMHDR* /*pnmh*/)
{
	//auto& nmhdr = *reinterpret_cast<NMPROPERTYITEM*>(pnmh);

	//int iItem;
	//int iSubItem;
	//if (!m_grid.FindProperty(nmhdr.prop, iItem, iSubItem))
	//	return FALSE;
	//
	//if (iSubItem == 3)
	//{
	//	auto& bkColor = dynamic_cast<CPropertyColorItem&>(*m_grid.GetProperty(iItem, 4));
	//	auto& txColor = dynamic_cast<CPropertyColorItem&>(*m_grid.GetProperty(iItem, 5));
	//	bkColor.SetEnabled(GetFilterType(iItem) != FilterType::Exclude);
	//	txColor.SetEnabled(GetFilterType(iItem) != FilterType::Exclude);
	//}

	//if (iSubItem == 4)
	//{
	//	auto& color = dynamic_cast<CPropertyColorItem&>(*nmhdr.prop);
	//	auto& edit = dynamic_cast<CPropertyEditItem&>(*m_grid.GetProperty(iItem, 1));
	//	edit.SetBkColor(color.GetColor());
	//	return TRUE;
	//}

	//if (iSubItem == 5)
	//{
	//	auto& color = dynamic_cast<CPropertyColorItem&>(*nmhdr.prop);
	//	auto& edit = dynamic_cast<CPropertyEditItem&>(*m_grid.GetProperty(iItem, 1));
	//	edit.SetTextColor(color.GetColor());
	//	return TRUE;
	//}

	return 0;
}

bool CSourcesPageImpl::GetSourceEnable(int iItem) const
{
	CComVariant val;
	GetGridItem<CPropertyCheckButtonItem>(m_grid, iItem, 0).GetValue(&val);
	return val.boolVal != VARIANT_FALSE;
}

std::wstring CSourcesPageImpl::GetSourceText(int iItem) const
{
	return GetGridItemText(m_grid, iItem, 1);
}

SourceType::type CSourcesPageImpl::GetSourceType(int iItem) const
{
	return StringToSourceType(Str(GetGridItemText(m_grid, iItem, 2)));
}

void CSourcesPageImpl::SetLogSources(const std::vector<std::shared_ptr<LogSource>>& logsources)
{
	m_logsources = logsources;
	if (IsWindow())
	{
		UpdateGrid();
	}
}

std::vector<std::shared_ptr<LogSource>> CSourcesPageImpl::GetSourcesToDelete() const
{
	return m_deleteSources;
}

void CSourcesPageImpl::UpdateGrid()
{
	//m_grid.DeleteAllItems();
	//for (auto it = m_filters.begin(); it != m_filters.end(); ++it)
	//	AddFilter(*it);
}

} // namespace debugviewpp 
} // namespace fusion
