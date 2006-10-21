
all: object.xml widget_xml.c
	gcc -Wall -g -o widget_xml widget_xml.c -lexpat -lxynth
	./widget_xml -f object.xml -o object
	gcc -Wall -g -o object object.c object_xml.c -lwidget -lxynth

clean:
	rm -rf object object_xml.c object_xml.h widget_xml
