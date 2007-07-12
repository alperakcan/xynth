
#include <config.h>
#include "gdk.h"
#include "gdkwindow.h"

#include "gdkregion-generic.h"

#include "gdkinternals.h"
#include "gdkalias.h"

#include "gdkxynth.h"
#include "gdkprivate-xynth.h"

static void gdk_drawable_impl_xynth_class_init (GdkDrawableImplXYNTHClass *klass)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

GType gdk_drawable_impl_xynth_get_type (void)
{
	static GType object_type = 0;
	ENTER();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof (GdkDrawableImplXYNTHClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gdk_drawable_impl_xynth_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof (GdkDrawableImplXYNTH),
			0,              /* n_preallocs */
			(GInstanceInitFunc) NULL,
		};
		object_type = g_type_register_static(GDK_TYPE_DRAWABLE, "GdkDrawableImplXYNTH", &object_info, 0);
	}
	LEAVE();
	return object_type;
}
