set PROJ_NAME VitisRho_pointops
open_project $PROJ_NAME
add_files point_ops_163.cpp
add_files gf163_ops.cpp

add_files -tb point_ops_tb.cpp \
    -cflags "-include /usr/local/include/gmp.h -I/usr/local/include -I../RhoPollard_lib/include"
add_files -tb ../RhoPollard_lib/src/Curve.cpp \
    -cflags "-include /usr/local/include/gmp.h -I/usr/local/include -I../RhoPollard_lib/include"
add_files -tb ../RhoPollard_lib/src/Point.cpp \
    -cflags "-include /usr/local/include/gmp.h -I/usr/local/include -I../RhoPollard_lib/include"
add_files -tb ../RhoPollard_lib/src/math_operations.cpp \
    -cflags "-include /usr/local/include/gmp.h -I/usr/local/include -I../RhoPollard_lib/include"

open_solution "solution1" -flow_target vitis
set_part {xc7z020clg400-1}
create_clock -period 10 -name default
set_top point_add_163
csim_design   -ldflags "-L/usr/local/lib -lgmpxx -lgmp"
csynth_design
