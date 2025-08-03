module oled_display_26x26(width = 26, height = 26) {
    thickness_error = 0.4;
    display_width = width + thickness_error;
    display_height = height + thickness_error;
    thickness = 4; // grosor típico del PCB
    hole_diameter = 1.7;
    hole_offset = 2.3; // distancia del borde al centro del agujero
    lcd_block_height = 13.5 + thickness_error;
    lcd_block_width = 25 + thickness_error;
    lcd_border_radius = 5;


    // LCD body
    difference() {

        union() {
            // Cuerpo del display
            color("black")
            cube([display_width , display_height, thickness*0.90]);

            // LCD
            color("gray")
                translate([(display_width - lcd_block_width +lcd_border_radius) /2, 
                            4.5 + lcd_border_radius/2,  
                            thickness*0.90 - 0.01 ])

                    linear_extrude(height = thickness*0.25) 
                        minkowski()
                        {
                            square([lcd_block_width - lcd_border_radius, 
                            lcd_block_height - lcd_border_radius]);
                            circle(d=lcd_border_radius, $fn=100);
                        }
        }

        //
        // Agujeros para tornillos (4 esquinas)
        //
        
        // up-Left
        translate([hole_offset, display_height - hole_offset, -0.1])
            color("silver")
            cylinder(h = 3*thickness, d = hole_diameter - thickness_error, $fn=50);
        // up-right
        translate([display_width - hole_offset, display_height - hole_offset, -0.1])
            color("silver")
            cylinder(h = 3*thickness, d = hole_diameter - thickness_error, $fn=50);
        // down-left
        translate([ hole_offset, hole_offset, -0.1])
            color("silver")
            cylinder(h = 3*thickness, d = hole_diameter - thickness_error, $fn=50);
        // down-right
        translate([display_width - hole_offset, hole_offset - thickness_error, -0.1])
            color("silver")
            cylinder(h = 3*thickness + 0.2, d = hole_diameter - thickness_error, $fn=50);
    }

}

/*

if (0)
{
    difference() 
    { 
        linear_extrude(height = 4) 
            minkowski() {
                square([28,28]);
                circle(d=1);
            }
        
        translate([1,1,-0.5]) 
        oled_display_26x26();
    }

}
else
{
    oled_display_26x26();
}

*/