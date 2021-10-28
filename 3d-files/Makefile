# make -j for multi threaded
# match "module foobar() { // `make` me"
#TARGETS=$(shell sed '/^module [a-z0-9_-]*().*make..\?me.*$$/!d;s/module //;s/().*/.stl/' print.scad)

all: stl buttons controll_base printTest throttlePot
	
buttons: button1 button2 button3 button4 button5 button6 button7 button8 button9 button0 buttonA buttonB buttonC buttonD buttonT1 buttonT2

%:
	openscad -m make -o stl/$@.stl -D "$@();" print.scad

stl:
	mkdir stl
