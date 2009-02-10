
-include ../../.config

target_so-$(CONFIG_GUIOO_SHARED) = libguioo.so

libguioo.so_includes-y = ./ ../lib
libguioo.so_files-y = \
	GuiAnimation.cpp \
	GuiEditBox.cpp \
	GuiImage.cpp \
	GuiPrimitive.cpp \
	GuiScrollBar.cpp \
	GuiTemplate.cpp \
	GuiWindow.cpp \
	GuiButton.cpp \
	GuiEffectWindow.cpp \
	GuiImageCache.cpp \
	GuiProgressBar.cpp \
	GuiScrollWindow.cpp \
	GuiTextBox.cpp \
	GuiWindowWorker.cpp \
	GuiCheckBox.cpp \
	GuiFrame.cpp \
	GuiMultiTextBox.cpp \
	GuiRectangle.cpp \
	GuiSurface.cpp \
	GuiTextMatrix.cpp \
	GuiAnimation.h \
	GuiEditBox.h \
	GuiImage.h \
	GuiPrimitive.h \
	GuiScrollBar.h \
	GuiTemplate.h \
	GuiWindow.h \
	GuiButton.h \
	GuiEffectWindow.h \
	GuiImageCache.h \
	GuiProgressBar.h \
	GuiScrollWindow.h \
	GuiTextBox.h \
	GuiWindowWorker.h \
	GuiCheckBox.h \
	GuiFrame.h \
	GuiMultiTextBox.h \
	GuiRectangle.h \
	GuiSurface.h \
	GuiTextMatrix.h

libguioo.so_libraries-y = ../lib/
libguioo.so_ldflags-y   = -lxynth

target_a-$(CONFIG_GUIOO_STATIC) = libguioo.a

libguioo.a_includes-y = $(libguioo.so_includes-y)
libguioo.a_files-y    = $(libguioo.so_files-y)

include ../../Makefile.lib

dist:
	$(CP) libguioo.so ../../dist/$(CONFIG_PATH_LIB)
	$(CP) libguioo.a ../../dist/$(CONFIG_PATH_LIB)
