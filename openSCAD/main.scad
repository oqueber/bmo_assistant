include <BOSL2/std.scad>
include <components/lcd128x64/lcd_screen.scad>
include <body.scad>

head_chamfer = 1.7;
head_thickness = 1.5;
head_border_radius = 5;
head_height = 23;
head_width =  32;

lcd_screem_height = 17;
lcd_screem_width = 25;
lcd_screem_thickness = 1.5;

lcd_outside_width = 24;
lcd_outside_height = 13;
lcd_outside_radius = 2;
lcd_outside_offset = 3;

module head_cubic (){

    difference() 
    {

        // HEAD
        cuboid([head_width,head_width,head_height], chamfer=head_chamfer);

        // HEAD HOLD
        move([0,0,-(head_height * .12)])
        scale(  1 - ((head_thickness * 2 * 100)/  head_width) / 100)
        cuboid([head_width ,head_width,head_height * 1.1], chamfer=head_chamfer);

        
        yrot(180)
        zrot(270)
        move([-26/2,-26/2 + 1,-11])
        oled_display_26x26();

        // FLAT BOTTOM
        move([0,0,-(head_height/2)])
        cube([head_width*2,head_width*2, 3*head_thickness], center = true);

        // LCD INNER
        move([head_width/2 - lcd_screem_thickness*1.55 ,0,1.5])
        cube([lcd_screem_thickness*2 ,lcd_screem_width , lcd_screem_height ] , center = true);

        //LCD OUTTER
        move([head_width/2  + lcd_screem_thickness ,0,lcd_outside_offset/2 -1.5])
        minkowski() {
            cube([
                lcd_screem_thickness, 
                lcd_outside_width - 2 * lcd_outside_radius, 
                lcd_outside_height - 2 * lcd_outside_radius
            ], center = true);
            sphere(lcd_outside_radius);
        }
    }


    move([0,0,-10])
    color("red")
    difference()
    {
        union()
        {
            // Tapa completa solida de 2 niveles
            translate([0, 0, -head_thickness - 0.1])
            diff()
                cube([head_width,head_width, 1.5*head_thickness], center=true) {
                    edge_mask(BOTTOM)
                        chamfer_edge_mask(l=50, chamfer=head_chamfer);
                    corner_mask(BOTTOM)
                        chamfer_corner_mask(chamfer=head_chamfer);
                }

            scale(  1 - (((head_thickness * 2.07 * 100)/  head_width) / 100))
            diff()
                cube([head_width,head_width,head_thickness], center=true) {
                    corner_mask(TOP+FRONT)
                        chamfer_edge_mask(l=50, chamfer=head_chamfer);
                    corner_mask(TOP+LEFT)
                        chamfer_edge_mask(l=50, chamfer=head_chamfer);
                    corner_mask(BOTTOM+RIGHT)
                        chamfer_edge_mask(l=50, chamfer=head_chamfer);
                    corner_mask(BOTTOM+LEFT)
                        chamfer_edge_mask(l=50, chamfer=head_chamfer);
            }
        }

        scale(  1 - (((2*head_thickness * 2 * 100)/  head_width) / 100) - 0.001)
        diff()
            cube([head_width,head_width, 2*head_thickness], center=true) {
                corner_mask(TOP+FRONT)
                    chamfer_edge_mask(l=50, chamfer=head_chamfer);
                corner_mask(TOP+LEFT)
                    chamfer_edge_mask(l=50, chamfer=head_chamfer);
                corner_mask(BOTTOM+RIGHT)
                    chamfer_edge_mask(l=50, chamfer=head_chamfer);
                corner_mask(BOTTOM+LEFT)
                    chamfer_edge_mask(l=50, chamfer=head_chamfer);
        }

        move([0,-7,0])
        cylinder(h = head_thickness * 4 , d = 5, center=true);

        cylinder(h = head_thickness * 4 , d = 4.9, center=true);
    }
}


body_cubic();

zrot(270)
translate([0 ,0, 30])  
    head_cubic();

    
