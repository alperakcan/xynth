#ifndef GUIIMAGECACHE_H_
#define GUIIMAGECACHE_H_

#include <xynth.h>

#include <map>
#include <string>

struct IMAGE_CACHE_ENTRY
{
	s_image_t	*pImgData;
	int			nRefs;
};

/**
 * \brief Image cache implementation
 * 
 * Given class implements the graphical image cache. Calling the static ope
 * method opens an image file. If it is open already, then the existing copy 
 * is returned and reference counter is incremented. Similarly, when the image 
 * is closed, the counter is decremented and when it reaches zero, image 
 * resources are released.  
 * 
 * \author Andrei Birjukov
 */

class GuiImageCache
{
public:
	
	static bool init();
	static bool release();
	
	static s_image_t* openImage(const char* fileName);
	static void closeImage(s_image_t* imageData);
	static unsigned int memUsage();

private:
	
	static std::map<std::string, IMAGE_CACHE_ENTRY*> imageMap;
	static s_thread_mutex_t	*mapMutex;
};

#endif /*GUIIMAGECACHE_H_*/
