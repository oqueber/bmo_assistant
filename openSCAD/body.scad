$fn = 100; // suavidad

include <BOSL2/std.scad>

module body_cubic(width = 35, height = 30)
{
    // Cuerpo
    body_thickness = 1.2;
    body_chamfer = 1.7;
    body_height = height;
    body_width =  width;

    usb_c_radius = 2;
    usb_c_width = 9.5 - 2*usb_c_radius;    // ancho típico del conector USB-C
    usb_c_hight = 4.1 - 2*usb_c_radius;    // alto típico

    //
    // BTN
    //

    num_buttons = 3;            // Número de botones
    button_size = 6.4;          // Tamaño del botón
    button_spacing = 3;         // Espacio entre ellos (ajustable)
    total_width = num_buttons * button_size + (num_buttons - 1) * button_spacing;
    start_x = - ((body_width /2 ) - body_thickness - 2*body_chamfer -4);

    //
    // SD
    //

    sd_width = 14;   
    sd_hight = 2.5;  

    union() 
    {
        difference() {
            // Cuerpo exterior con esquinas redondeadas
            cuboid([body_width,body_width,body_height], chamfer=body_chamfer);

            // Corte para dejar cabeza plana
            translate([0, 0, body_height/2 - 2*body_thickness]) 
                linear_extrude(height = 2 * body_height) {
                    square([2 * body_width, 2 * body_width], center = true);
                }

            // hueco interior
            move([0,0,(body_height * .1)])
            scale(  1 - ((body_thickness * 2 * 100)/  body_width) / 100)
            cuboid([body_width,body_width,body_height], chamfer=body_chamfer);

            // USB - C
            translate([-(body_width/2 - usb_c_width - 7*body_thickness), body_width/2 + body_thickness/2, -body_height/2 + usb_c_radius + usb_c_hight + 4*body_thickness]) 
                rotate([90,0,0])
                    linear_extrude(height = 4*body_thickness) {
                        minkowski() {
                            square([usb_c_width, usb_c_hight], center = true);
                            circle(usb_c_radius);
                        }
                    }

            translate([-(body_width/2 - 4), body_width/2 - 4*body_thickness, -body_height/2 + usb_c_radius + usb_c_hight + 1.5*body_thickness]) 
            cube([ 20, 4, 4]);
            
            // micro-sd
            color("red")
            translate([-(body_width/2 - sd_hight - 1*body_thickness), body_width/2 - body_thickness/2, 3]) 
                rotate([0,90,0])
                    linear_extrude(height = 2*body_thickness) {
                        square([sd_width, sd_hight], center = true);
                    }

            //  Botones
            for (i = [0:num_buttons - 1])
                translate([
                    start_x + i * (button_size + button_spacing),
                    -body_width/2,
                    0
                ])
                    cube([button_size, button_size, button_size], center = true);

        }
    
        translate([-(body_width/2  - 3), body_width/2 - body_thickness/2 - 16, 3 + 7.5]) 
        rotate([0,90,0])
        cylinder(h = 4, d = 2, center = true);

        translate([-(body_width/2 - 3), body_width/2 - body_thickness/2 - 16, 3 - 7.5]) 
        rotate([0,90,0])
        cylinder(h = 4, d = 2, center = true);
    }


    translate([0, 0, body_height/2 - 1 ])
    color("blue")
    difference()
    {
        union()
        {
            // Tapa completa solida de 2 niveles
            translate([0, 0, body_thickness - 0.1])
            diff()
                cube([body_width,body_width,1.3*body_thickness], center=true) {
                    edge_mask(TOP)
                        chamfer_edge_mask(l=50, chamfer=body_chamfer/2);
                    corner_mask(TOP)
                        chamfer_corner_mask(chamfer=body_chamfer);
            }

            scale(  1 - (((body_thickness * 2.05 * 100)/  body_width) / 100) - 0.001)
            diff()
                cube([body_width,body_width,body_thickness], center=true) {
                    corner_mask(TOP+FRONT)
                        chamfer_edge_mask(l=50, chamfer=body_chamfer);
                    corner_mask(TOP+LEFT)
                        chamfer_edge_mask(l=50, chamfer=body_chamfer);
                    corner_mask(BOTTOM+RIGHT)
                        chamfer_edge_mask(l=50, chamfer=body_chamfer);
                    corner_mask(BOTTOM+LEFT)
                        chamfer_edge_mask(l=50, chamfer=body_chamfer);
            }
        }

        translate([0, 0, -0.1])
        scale(  1 - (((2*body_thickness * 2 * 100)/  body_width) / 100) - 0.001)
        diff()
            cube([body_width,body_width,body_thickness], center=true) {
                corner_mask(TOP+FRONT)
                    chamfer_edge_mask(l=50, chamfer=body_chamfer);
                corner_mask(TOP+LEFT)
                    chamfer_edge_mask(l=50, chamfer=body_chamfer);
                corner_mask(BOTTOM+RIGHT)
                    chamfer_edge_mask(l=50, chamfer=body_chamfer);
                corner_mask(BOTTOM+LEFT)
                    chamfer_edge_mask(l=50, chamfer=body_chamfer);
        }

        // Hueco para la conexión con
        translate([0, 4, 0])
            cube( [12,22.5,10], center= true);

        translate([-8 , 0, 0])
            cylinder( d=5, h=7, center= true);

    }
}


// tapa con la cabeza del servo
/*
translate([0, 0, body_height/2 - 2 ])
    difference()
    {
        union()
        {
            // Tapa completa solida de 2 niveles
            translate([0, 0, body_thickness])
            diff()
                cube([body_width,body_width,body_thickness], center=true) {
                    corner_mask(TOP+FRONT)
                        chamfer_edge_mask(l=50, chamfer=body_chamfer);
                    corner_mask(TOP+LEFT)
                        chamfer_edge_mask(l=50, chamfer=body_chamfer);
                    corner_mask(BOTTOM+RIGHT)
                        chamfer_edge_mask(l=50, chamfer=body_chamfer);
                    corner_mask(BOTTOM+LEFT)
                        chamfer_edge_mask(l=50, chamfer=body_chamfer);
            }

            scale(  1 - (((body_thickness * 2.05 * 100)/  body_width) / 100) - 0.001)
            diff()
                cube([body_width,body_width,body_thickness], center=true) {
                    corner_mask(TOP+FRONT)
                        chamfer_edge_mask(l=50, chamfer=body_chamfer);
                    corner_mask(TOP+LEFT)
                        chamfer_edge_mask(l=50, chamfer=body_chamfer);
                    corner_mask(BOTTOM+RIGHT)
                        chamfer_edge_mask(l=50, chamfer=body_chamfer);
                    corner_mask(BOTTOM+LEFT)
                        chamfer_edge_mask(l=50, chamfer=body_chamfer);
            }
        }

        translate([0, 0, -0.1])
        scale(  1 - (((2*body_thickness * 2 * 100)/  body_width) / 100) - 0.001)
        diff()
            cube([body_width,body_width,body_thickness], center=true) {
                corner_mask(TOP+FRONT)
                    chamfer_edge_mask(l=50, chamfer=body_chamfer);
                corner_mask(TOP+LEFT)
                    chamfer_edge_mask(l=50, chamfer=body_chamfer);
                corner_mask(BOTTOM+RIGHT)
                    chamfer_edge_mask(l=50, chamfer=body_chamfer);
                corner_mask(BOTTOM+LEFT)
                    chamfer_edge_mask(l=50, chamfer=body_chamfer);
        }

        // Hueco para la conexión con
        translate([0, 0, 0])
            cylinder( d=11.7, h=7, center= true);

        translate([0 , 6, 0])
            cylinder( d=5.4, h=7, center= true);


    }*/
//body_cubic();