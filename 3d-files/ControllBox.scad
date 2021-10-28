// Controll box for arduino train controller

margin = 0.25;

base_x = 180;
base_y = 80;
base_z = 2.5;

button_dist_x = 44.3/3;
button_dist_y = 24.2/3;

button_x = 8;
button_x2 = button_dist_x - 1 - margin*4;
button_y = 6;
button_h1 = 1;
button_h2 = button_h1+base_z+1.5;

button_offset_y = 35-36-button_y/2;

mount_dist_x = 65;
mount_dist_y = 64;

lcd_x = 60.5 + margin*2;
lcd_y = 14.1 + margin*2;
lcd_h = 7.2;
lcd_offset_y = 35-5-lcd_y/2;

pcb_x = 70;
pcb_y = 70;
pcb_h = 1.7;
pcb_b_x = 6;
pcb_b_y = 6;
pcb_b_h = 3.5;
pcb_b_h2 = 4.5;
pcb_spacers_h = pcb_b_h2+button_h1+margin;
pcb_spacers_r = 3.2;

stand_h = 15;

throttle_d1 = 36;
throttle_pot_d = 7+margin;

throttle_offset_x = base_x/2 - throttle_d1/2;
throttle_offset_y = base_y/2 - throttle_d1/2;
//translate([0-button_dist_x*1.5, 0-button_dist_y*3+button_offset_y, -button_h1-margin/2])button("1");

//base();
//%translate([0,0,-pcb_spacers_h]) TM();
module button(text1="0") {
    
    
    difference() {
        union() {
            translate([-button_x/2, -button_y/2]) cube([button_x, button_y, button_h2]);
            translate([-button_x2/2, -button_y/2]) cube([button_x2, button_y, button_h1]);
        }
        translate([0,0, button_h2-0.5])scale(0.5) linear_extrude(4) text(text1, valign="center", halign="center");
    }
}



module base() {
    difference() {
        union() {
            //translate([-base_x/2, -base_y/2]) cube([base_x, base_y, base_z]);
            base_box();
            
                    // spacers
            translate([ mount_dist_x/2, mount_dist_y/2,-pcb_spacers_h]) cylinder(r=pcb_spacers_r, h=pcb_spacers_h);
            translate([-mount_dist_x/2, mount_dist_y/2,-pcb_spacers_h]) cylinder(r=pcb_spacers_r, h=pcb_spacers_h);
            translate([mount_dist_x/2, -mount_dist_y/2,-pcb_spacers_h]) cylinder(r=pcb_spacers_r, h=pcb_spacers_h);
            translate([-mount_dist_x/2, -mount_dist_y/2,-pcb_spacers_h]) cylinder(r=pcb_spacers_r, h=pcb_spacers_h);
            
            
            // stand
            translate([mount_dist_x/2, -mount_dist_y/2-10,-stand_h]) cylinder(r=pcb_spacers_r, h=stand_h);
            translate([-mount_dist_x/2, -mount_dist_y/2-10,-stand_h]) cylinder(r=pcb_spacers_r, h=stand_h);
            
            translate([base_x/2 - pcb_spacers_r, throttle_offset_y,-stand_h]) cylinder(r=pcb_spacers_r, h=stand_h);
            translate([-base_x/2 + pcb_spacers_r, throttle_offset_y,-stand_h]) cylinder(r=pcb_spacers_r, h=stand_h);
            
        }
        translate([-throttle_offset_x+throttle_d1/2, throttle_offset_y-throttle_d1/2+2, base_z-0.3])scale(1) linear_extrude(4) text("T1", valign="bottom", halign="left");
        translate([throttle_offset_x-throttle_d1/2, throttle_offset_y-throttle_d1/2+2, base_z-0.3])scale(1) linear_extrude(4) text("T2", valign="bottom", halign="right");
        
        translate([0, (lcd_offset_y-lcd_y/2)/2, base_z-0.3])scale(0.7) linear_extrude(4) text("DCC-SAAC", valign="center", halign="center");
        
        b_x = button_x + margin*2;
        b_y = button_y + margin*2;
        for ( ix = [0 : button_dist_x : button_dist_x*3] ){
            for ( iy = [0 : button_dist_y : button_dist_y*3] ){
                translate([ix-button_dist_x*1.5, iy-button_dist_y*3+button_offset_y, -button_h1]) translate([-b_x/2, -b_y/2]) cube([b_x, b_y, button_h2]);
            }
        }
        translate([-lcd_x/2, -lcd_y/2+lcd_offset_y, -3]) cube([lcd_x, lcd_y, 10]);
        //translate([10, button_offset_y+button_y/2, -3]) cube([3, 16.6, 10]);
        
        // Throttles
        translate([throttle_offset_x, throttle_offset_y,-1]) cylinder(d=throttle_pot_d, h=20);
        translate([-throttle_offset_x, throttle_offset_y,-1]) cylinder(d=throttle_pot_d, h=20);
        % color("red")translate([throttle_offset_x, throttle_offset_y, base_z+1]) ratt();
        % color("red")translate([-throttle_offset_x, throttle_offset_y, base_z+1]) ratt();
        
        // holes
        translate([mount_dist_x/2, mount_dist_y/2,-pcb_h-10]) cylinder(d=3, h=20);
        translate([-mount_dist_x/2, mount_dist_y/2,-pcb_h-10]) cylinder(d=3, h=20);
        translate([mount_dist_x/2, -mount_dist_y/2,-pcb_h-10]) cylinder(d=3, h=20);
        translate([-mount_dist_x/2, -mount_dist_y/2,-pcb_h-10]) cylinder(d=3, h=20);
        translate([mount_dist_x/2, -mount_dist_y/2-10,-pcb_h-1]) cylinder(d=3, h=20);
        translate([-mount_dist_x/2, -mount_dist_y/2-10,-pcb_h-1]) cylinder(d=3, h=20);
        translate([0,0,-pcb_spacers_h]) TM();
        
        // stand
        translate([mount_dist_x/2, -mount_dist_y/2-10,-stand_h]) cylinder(d=3, h=stand_h);
        translate([-mount_dist_x/2, -mount_dist_y/2-10,-stand_h]) cylinder(d=3, h=stand_h);
        
        translate([base_x/2 - pcb_spacers_r, throttle_offset_y,-stand_h]) cylinder(d=3, h=stand_h);
        translate([-base_x/2 + pcb_spacers_r, throttle_offset_y,-stand_h]) cylinder(d=3, h=stand_h);
    }
    
    
}

//base_box();
module base_box() {
    wall = 2;
    difference() {
        union() {
            //translate([-base_x/2, -base_y/2]) cube([base_x, base_y, base_z]);
            hull() {
                translate([throttle_offset_x, throttle_offset_y , -stand_h]) cylinder(d=throttle_d1, h=base_z+stand_h);
                translate([-throttle_offset_x, throttle_offset_y, -stand_h]) cylinder(d=throttle_d1, h=base_z+stand_h);
            }
            hull() {
                translate([mount_dist_x/2, mount_dist_y/2, -stand_h]) cylinder(r=pcb_spacers_r*2, h=base_z+stand_h);
                translate([-mount_dist_x/2, mount_dist_y/2, -stand_h]) cylinder(r=pcb_spacers_r*2, h=base_z+stand_h);
                translate([mount_dist_x/2, -mount_dist_y/2-10, -stand_h]) cylinder(r=pcb_spacers_r*2, h=base_z+stand_h);
                translate([-mount_dist_x/2, -mount_dist_y/2-10, -stand_h]) cylinder(r=pcb_spacers_r*2, h=base_z+stand_h);
            }
        }
        union() {
            //translate([-base_x/2, -base_y/2]) cube([base_x, base_y, base_z]);
            hull() {
                translate([throttle_offset_x, throttle_offset_y , -stand_h-base_z]) cylinder(d=throttle_d1-wall*2, h=base_z+stand_h);
                translate([-throttle_offset_x, throttle_offset_y, -stand_h-base_z]) cylinder(d=throttle_d1-wall*2, h=base_z+stand_h);
            }
            hull() {
                translate([mount_dist_x/2, mount_dist_y/2, -stand_h-base_z]) cylinder(r=pcb_spacers_r*2-wall, h=base_z+stand_h);
                translate([-mount_dist_x/2, mount_dist_y/2, -stand_h-base_z]) cylinder(r=pcb_spacers_r*2-wall, h=base_z+stand_h);
                translate([mount_dist_x/2, -mount_dist_y/2-10, -stand_h-base_z]) cylinder(r=pcb_spacers_r*2-wall, h=base_z+stand_h);
                translate([-mount_dist_x/2, -mount_dist_y/2-10, -stand_h-base_z]) cylinder(r=pcb_spacers_r*2-wall, h=base_z+stand_h);
            }
        }
        
        // cable hole
        translate([-30/2, 0, -stand_h]) cube([30, base_y, 5]);
    }
}


module TM() {

    
    difference() {
        union() {
            color("green")translate([-pcb_x/2, -pcb_y/2, -pcb_h]) cube([pcb_x, pcb_y, pcb_h]);

            for ( ix = [0 : button_dist_x : button_dist_x*3] ){
                for ( iy = [0 : button_dist_y : button_dist_y*3] ){
                    color("lightgrey")translate([ix-button_dist_x*1.5, iy-button_dist_y*3+button_offset_y, 0]) translate([-pcb_b_x/2, -pcb_b_y/2]) cube([pcb_b_x, pcb_b_y, pcb_b_h]);
                    color("black")translate([ix-button_dist_x*1.5, iy-button_dist_y*3+button_offset_y, 0])  cylinder(d=3.6, h=pcb_b_h2);
                }
            }
            color("grey") translate([-lcd_x/2, -lcd_y/2+lcd_offset_y, 0]) cube([lcd_x, lcd_y, lcd_h]);
            
            // pinheaders
            color("yellow")translate([-mount_dist_x/2 - 1.5, -margin, 0]) cube([3, 13, 12]);
        }
        
        
        //translate([10, button_offset_y+button_y/2, -3]) cube([3, 16.6, 10]);
        
        // holes
        translate([mount_dist_x/2, mount_dist_y/2,-pcb_h-1]) cylinder(d=3, h=10);
        translate([-mount_dist_x/2, mount_dist_y/2,-pcb_h-1]) cylinder(d=3, h=10);
        translate([mount_dist_x/2, -mount_dist_y/2,-pcb_h-1]) cylinder(d=3, h=10);
        translate([-mount_dist_x/2, -mount_dist_y/2,-pcb_h-1]) cylinder(d=3, h=10);
    }
    
    
}

//ratt();
module ratt() {
    difference() {
        union() {
            cylinder(d1=throttle_d1, d2= throttle_d1-6, h= 5);
            hull() {
                translate([26,0,0]) cylinder(d=1, h=1);
                translate([0,0,0]) cylinder(d1=10, d2=7, h=14);
            }
            hull() {
                
                translate([0,0,0]) cylinder(d1=10, d2=7, h=14);
                translate([-20,0,0]) cylinder(d1=10, d2=7, h=14);
            }
        }
        
        cylinder(d=6, h=15-base_z);
        cylinder(d=12+margin*2, h=3);
    }
}
