
all: object.xml widget_xml.c
	gcc -Wall -g -o widget_xml widget_xml.c -lexpat -lxynth
	./widget_xml < object.xml > object.c
	gcc -o object object.c -lwidget -lxynth

clean:
	rm -rf object object.c widget_xml

