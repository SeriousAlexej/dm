#pragma once

#include "Main.hpp"

#define T_GUILD_LISTER_PARENT_CLASS TEXT("GuildListerParent")
#define T_GUILD_LISTER_CLASS TEXT("GuildLister")

// config
#define GUILD_LISTER_BORDER 1

#if GUILD_LISTER_BORDER
# define GUILD_LISTER_BORDER_SIZE 2
#else
# define GUILD_LISTER_BORDER_SIZE 0
#endif

class GuildLister
{
public:
	HWND m_hwnd            = NULL;
	HWND m_scrollable_hwnd = NULL;
	HWND m_tooltip_hwnd    = NULL;

	int m_scroll_pos = 0;

	Snowflake m_rightClickedGuild = 0;

public:
	GuildLister();
	~GuildLister();

public:
	void ProperlyResizeSubWindows();
	void ClearTooltips();
	void UpdateTooltips();
	void Update();
	void OnScroll();
	void ShowMenu(Snowflake guild, POINT pt);

public:
	static WNDCLASS g_GuildListerParentClass;
	static WNDCLASS g_GuildListerClass;

	static LRESULT CALLBACK ParentWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void InitializeClass();

	static GuildLister* Create(HWND hwnd, LPRECT pRect);

private:
	static int GetScrollableHeight();
};
