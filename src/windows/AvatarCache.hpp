#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <unordered_map>
#include <set>

#include "../discord/Snowflake.hpp"

enum class eImagePlace
{
	NONE,
	AVATARS,     // profile avatars
	ICONS,       // server icons
	ATTACHMENTS, // attachments
};

struct ImagePlace {
	eImagePlace type = eImagePlace::NONE;
	Snowflake sf = 0;
	std::string place;
	std::string key;
	
	ImagePlace() {}
	ImagePlace(eImagePlace t, Snowflake s, const std::string& p, const std::string& k) : type(t), sf(s), place(p), key(k) {}
};

#define HBITMAP_LOADING ((HBITMAP) 0xDDCCBBAA)
#define HBITMAP_ERROR   ((HBITMAP) 0xDDCCBBAB)

// Kind of a misnomer as it also handles attachment resources.
class AvatarCache
{
public:
	// Create a 32-character identifier based on the resource name.  If a 32 character
	// GUID was provided, return it, otherwise perform the MD5 hash of the string.
	std::string MakeIdentifier(const std::string& resource);

	// Let the avatar cache know where resource with the specified ID is located.
	void AddImagePlace(const std::string& resource, eImagePlace ip, const std::string& place, Snowflake sf = 0);

	// Set the bitmap associated with the resource ID.
	// Note, after this, hbm is owned by the profile bitmap handler, so you shouldn't delete it
	void SetBitmap(const std::string& resource, HBITMAP hbm);

	// Get the type of the resource with the specified ID.
	ImagePlace GetPlace(const std::string& resource);

	// Let the avatar cache know that the resource was loaded.
	void LoadedResource(const std::string& resource);

	// Get the bitmap associated with the resource.  If it isn't loaded, request it, and return special bitmap handles.
	HBITMAP GetBitmapSpecial(const std::string& resource);

	// Get the bitmap associated with the resource.  If it isn't loaded, request it, and return NULL.
	HBITMAP GetBitmapNullable(const std::string& resource);

	// Get the bitmap associated with the resource.  If it isn't loaded, request it, and return a default.
	HBITMAP GetBitmap(const std::string& resource);

	// Delete all bitmaps.
	void WipeBitmaps();

	// Erase the bitmap with the specified ID.  Make sure no one
	// else is holding a reference to it.
	void EraseBitmap(const std::string& resource);

	// Trim a single bitmap slot.  This makes room for another.
	bool TrimBitmap();

	// Trim X bitmap slots.  This makes room for others.
	int  TrimBitmaps(int num = 1);

	// Clear the processing requests set.  These requests will never be fulfilled.
	void ClearProcessingRequests();
	
private:
	// Cache for MakeIdentifier.  MD5 hashes aren't too cheap.
	std::unordered_map<std::string, std::string> m_resourceNameToID;

	// The value has two items: the bitmap itself and an 'age'. From time to time,
	// bitmaps are 'aged'; GetBitmap resets the age of the specific bitmap to zero.
	std::unordered_map<std::string, std::pair<HBITMAP, int>> m_profileToBitmap;

	// The place where the resource with the specified ID can be found.
	std::unordered_map<std::string, ImagePlace> m_imagePlaces;

	// A list of resources pending load.
	std::set<std::string> m_loadingResources;

	// Delete the bitmap if it isn't the default one.
	void DeleteBitmapIfNeeded(HBITMAP hbm);
};

AvatarCache* GetAvatarCache();
