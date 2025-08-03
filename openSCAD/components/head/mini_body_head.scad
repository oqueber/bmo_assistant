$fn = 100; // suavidad


include <../../components/lcd128x64/lcd_screen.scad> 

include <BOSL2/std.scad>

head_thickness = 1.5;
head_border_radius = 5;
head_height = 30;
head_width =  30;




cuboid([head_width,head_width,head_height], chamfer=1);

#move([26/2, 26/2, 26/2]) 
yrot(90)
zrot(270)
oled_display_26x26(26,26);

