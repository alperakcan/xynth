
all: object.xml widget_xml.c
	gcc -Wall -g -o widget_xml widget_xml.c -lexpat -lxynth
	./widget_xml < object.xml > object_xml.c
	gcc -o object object.c object_xml.c -lwidget -lxynth

clean:
	rm -rf object object_xml.c widget_xml

