
var hidden = 0;

function rand (n)
{
	return (Math.floor(Math.random() * n + 1));
}

function frame_red_draw (object)
{
	js_objectFillBox(object, 0, 0, js_objectGetWidth(object), js_objectGetHeight(object), 255, 0, 0);
}

function frame_green_draw (object)
{
	js_objectFillBox(object, 0, 0, js_objectGetWidth(object), js_objectGetHeight(object), 0, 255, 0);
}

function frame_blue_draw (object)
{
	js_objectFillBox(object, 0, 0, js_objectGetWidth(object), js_objectGetHeight(object), 0, 0, 255);
}

function frame_white_draw (object)
{
	js_objectFillBox(object, 0, 0, js_objectGetWidth(object), js_objectGetHeight(object), 255, 255, 255);
}

function button_hide_draw (object)
{
	var textbox_hide = js_getElementById("textbox_hide");
	js_buttonDraw(object);
	if (hidden == 0) {
		js_textboxSetStr(textbox_hide, "hide area");
	} else {
		js_textboxSetStr(textbox_hide, "show area");
	}
}

function button_hide_pressed (object, button)
{
	var area = js_getElementById("area");
	if (hidden == 0) {
		hidden = 1;
		js_objectHide(area);
	} else {
		hidden = 0;
		js_objectShow(area);
	}
}

function button_blender_pressed (object, button)
{
	var area = js_getElementById("area");
	var frame_red = js_getElementById("frame_red");
	var frame_green = js_getElementById("frame_green");
	var frame_blue = js_getElementById("frame_blue");
	var frame_white = js_getElementById("frame_white");
	js_objectMove(frame_red, rand(js_objectGetWidth(area)) + 1, rand(js_objectGetHeight(area)) + 1, rand(js_objectGetWidth(area)) + 1, rand(js_objectGetHeight(area)) + 1);
	js_objectMove(frame_green, rand(js_objectGetWidth(area)) + 1, rand(js_objectGetHeight(area)) + 1, rand(js_objectGetWidth(area)) + 1, rand(js_objectGetHeight(area)) + 1);
	js_objectMove(frame_blue, rand(js_objectGetWidth(area)) + 1, rand(js_objectGetHeight(area)) + 1, rand(js_objectGetWidth(area)) + 1, rand(js_objectGetHeight(area)) + 1);
	js_objectMove(frame_white, rand(js_objectGetWidth(area)) + 1, rand(js_objectGetHeight(area)) + 1, rand(js_objectGetWidth(area)) + 1, rand(js_objectGetHeight(area)) + 1);
}

function button_focus_pressed (object, button)
{
	var r = rand(4);
	var frame_red = js_getElementById("frame_red");
	var frame_green = js_getElementById("frame_green");
	var frame_blue = js_getElementById("frame_blue");
	var frame_white = js_getElementById("frame_white");
	switch (r) {
		case 1: js_objectShow(frame_red); break;
		case 2: js_objectShow(frame_green); break;
		case 3: js_objectShow(frame_blue); break;
		case 4: js_objectShow(frame_white); break;
	}
}
