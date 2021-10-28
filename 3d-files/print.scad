// This will batch generate all stl files for the make all command. Rotate to print direction

use <ControllBox.scad>
include <ControllBox.scad>;

param1=0;   // must be initalised
len=param1; // param1 passed via -D on cmd-line
echo(len,param1);

$fs = 0.5;
$fa = 5.1;

//printTest();

module controll_base() {
    rotate([180,0,0]) base();
}

module throttlePot() {
    ratt();
}
module printTest() {
    translate([-20,20]) button("T1");
    translate([-20,10]) button("B");
    base_x = 35;
    base_y = 28;
    difference() {
        union() {
            translate([-33, -38]) cube([base_x, base_y, base_z]);
            //translate([-button_x2/2, -button_y/2]) cube([button_x2, button_y, button_h1]);
        }
        
        b_x = button_x + margin*2;
        b_y = button_y + margin*2;
        //translate([0, 0, -button_h1]) translate([-b_x/2, -b_y/2]) cube([b_x, b_y, button_h2]);
        for ( ix = [0 : button_dist_x : button_dist_x*1] ){
            for ( iy = [0 : button_dist_y : button_dist_y*1] ){
                translate([ix-button_dist_x*1.5, iy-button_dist_y*3+button_offset_y, -button_h1]) translate([-b_x/2, -b_y/2]) cube([b_x, b_y, button_h2]);
            }
        }
            

    }
    
}
module button1() { 
    button("1");
}
module button2() { 
    button("2");
}
module button3() { 
    button("3");
}
module button4() { 
    button("4");
}
module button5() { 
    button("5");
}
module button6() { 
    button("6");
}
module button7() { 
    button("7");
}
module button8() { 
    button("8");
}
module button9() { 
    button("9");
}
module button0() { 
    button("0");
}
module buttonA() { 
    button("A");
}
module buttonB() { 
    button("B");
}
module buttonC() { 
    button("C");
}
module buttonD() { 
    button("D");
}
module buttonT1() { 
    button("T1");
}
module buttonT2() { 
    button("T2");
}

module buttons() {
    cube(1);
    
}
module all() {
    cube(1);
    
}