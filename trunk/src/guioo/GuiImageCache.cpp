/***************************************************************************
 *   Copyright            : (c) 2007-2008 Artec Design LLC                 *
 *                        : (c) 2007-2008 Andrei Birjukov                  *
 *                        : (c) 2007-2008 Anti Sullin                      *
 *   Contact              : andrei.birjukov at artecdesign dot ee          *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include "GuiImageCache.h"

std::map<std::string, IMAGE_CACHE_ENTRY*> GuiImageCache::imageMap;
s_thread_mutex_t* GuiImageCache::mapMutex = NULL;

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Open image file
 *
 * Opens the specified image file. If the file was opened before, returns the
 * cached object from the list.
 *
 * \param fileName Path to image file in the filesystem
 * \return Returns image descriptor if the file was successfully opened, or
 * NULL if open procedure failed.
 */
s_image_t* GuiImageCache::openImage(const char* fileName)
{
	if(mapMutex == NULL) return NULL;
	s_thread_mutex_lock(mapMutex);

	s_image_t* imgData = NULL;

	std::map<std::string, IMAGE_CACHE_ENTRY*>::iterator it;
	it = imageMap.find(fileName);

	// image not found in the cache
	if(it == imageMap.end())
	{
		s_image_init(&imgData);
		// open image file, abort on failure
		if(s_image_img((char*)fileName, imgData) != 0)
		{
			s_image_uninit(imgData);
			s_thread_mutex_unlock(mapMutex);
			return NULL;
		}

		IMAGE_CACHE_ENTRY *pEntry = new IMAGE_CACHE_ENTRY;
		pEntry->pImgData = imgData;
		pEntry->nRefs = 1;

		// insert the new entry to the map
		imageMap[fileName] = pEntry;

		#ifdef DEBUG
		printf("GuiImageCache::openImage: creating new entry %x for %s\n",
			(unsigned int)pEntry->pImgData, fileName);
		#endif
	}
	else	// image is already in the map, return existing one
	{
		IMAGE_CACHE_ENTRY *pEntry = it->second;
		imgData = pEntry->pImgData;
		pEntry->nRefs ++;

		#ifdef DEBUG
		printf("GuiImageCache::openImage: reusing entry %x for %s\n",
			(unsigned int)pEntry->pImgData, fileName);
		#endif
	}

	s_thread_mutex_unlock(mapMutex);

	#ifdef DEBUG
	printf("GuiImageCache: memory usage %u bytes / %d entries\n",
		memUsage(), imageMap.size());
	#endif

	return imgData;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Close image
 *
 * Closes the specified image. Image resources will be released if it is not
 * referenced anymore.
 *
 * \param imageData pointer to the image descriptor
 */
void GuiImageCache::closeImage(s_image_t* imageData)
{
	if(mapMutex == NULL || imageData == NULL) return;
	s_thread_mutex_lock(mapMutex);

	#ifdef DEBUG
	printf("GuiImageCache::closeImage: closing image %x\n", (unsigned int)imageData);
	#endif

	std::map<std::string, IMAGE_CACHE_ENTRY*>::iterator it;
	for(it = imageMap.begin(); it != imageMap.end(); it ++)
	{
		IMAGE_CACHE_ENTRY *pEntry = (IMAGE_CACHE_ENTRY*)it->second;
		if(pEntry->pImgData == imageData)
		{
			// if there are no more users of that resource remove it
			if((-- pEntry->nRefs) == 0)
			{
				#ifdef DEBUG
				printf("GuiImageCache::closeImage: removing entry %x\n", (unsigned int)pEntry->pImgData);
				#endif
				s_image_uninit(pEntry->pImgData);
				delete pEntry;
				imageMap.erase(it);
			}

			break;
		}
	}

	s_thread_mutex_unlock(mapMutex);

	#ifdef DEBUG
	printf("GuiImageCache: memory usage %u bytes / %d entries\n",
		memUsage(), imageMap.size());
	#endif
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Initialize image cache
 *
 * Initializes image cache
 *
 * \return Returns true if the initialization if successful.
 */
bool GuiImageCache::init()
{
	if(s_thread_mutex_init(&mapMutex) != 0) return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Close image cache
 *
 * Deinitializes image cache. Also releases any remaining open images.
 *
 * \return Returns true if image cache is deinitialized successfully
 */
bool GuiImageCache::release()
{
	if(mapMutex == NULL) return false;
	s_thread_mutex_lock(mapMutex);

	std::map<std::string, IMAGE_CACHE_ENTRY*>::iterator it;
	for(it = imageMap.begin(); it != imageMap.end(); it ++)
	{
		IMAGE_CACHE_ENTRY *pEntry = (IMAGE_CACHE_ENTRY*)it->second;
		if(pEntry->pImgData) s_image_uninit(pEntry->pImgData);
		delete pEntry;
	}

	imageMap.clear();

	s_thread_mutex_destroy(mapMutex);
	mapMutex = NULL;
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get memory usage
 *
 * Retrives the current amount of memory used by the image cache
 *
 * \return Returns number of bytes allocated by the cache.
 */
unsigned int GuiImageCache::memUsage()
{
	if(mapMutex == NULL) return 0;
	s_thread_mutex_lock(mapMutex);
	unsigned long size = 0;

	std::map<std::string, IMAGE_CACHE_ENTRY*>::iterator it;
	for(it = imageMap.begin(); it != imageMap.end(); it ++)
	{
		IMAGE_CACHE_ENTRY *pEntry = (IMAGE_CACHE_ENTRY*)it->second;
		if(pEntry->pImgData == NULL) continue;

		size += pEntry->pImgData->w * pEntry->pImgData->h * 4;
	}

	s_thread_mutex_unlock(mapMutex);
	return size;
}
