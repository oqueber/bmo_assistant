$fn = 100; // suavidad

module head(
    head_thickness = 1.5,
    head_height = 31,
    head_width =  30,
    head_border_radius = 10,
)
{
    difference(){

        difference(){
            translate([head_border_radius,head_border_radius,0])
            linear_extrude(scale=0.8, height = head_height) {
                minkowski() {
                    square([
                        head_width - 2*head_border_radius, 
                        head_width - 2*head_border_radius
                    ]);
                    circle(head_border_radius);
                }
            }

            translate([head_width/2.7,5,0])
            rotate([90,0,0])
                linear_extrude(height=2*head_width, center=true) {
                    difference()  {square(2*head_height);resize([ head_width * 1.3 , head_height * 2]) circle(d=20);} // square and not circle
                }

        }


        translate([1,1,-1])
        scale(0.95)
        {
            difference()
            {
                translate([head_border_radius,head_border_radius,0])
                    linear_extrude(scale=0.8, height = head_height) {
                        minkowski() {
                            square([
                                head_width - 2*head_border_radius, 
                                head_width - 2*head_border_radius
                            ]);
                            circle(head_border_radius);
                        }
                    }

                translate([head_width/2.7,5,0])
                    rotate([90,0,0])
                        linear_extrude(height=2*head_width, center=true) {
                            difference()  {square(2*head_height);resize([ head_width * 1.3 , head_height * 2]) circle(d=20);} // square and not circle
                        }

            }
        }
    }
}
