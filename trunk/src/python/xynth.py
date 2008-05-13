import xynth;

def xynh_atevent_cb (window, event):
  print('atevent callback works!');

def xynth_atexit_cb (window):
  print('atexit callback works!');
  
def xynth_test ():
  w = xynth.window(xynth.WINDOW_TYPE_MAIN);
  w.set_title('python example works!');
  w.set_coor(xynth.WINDOW_TYPE_NOFORM, 50, 50, 400, 100);
  w.atevent(xynh_atevent_cb);
  w.atexit(xynth_atexit_cb);
  w.show();
  w.main();

atevent_cnt = 0;

xynth_test();
