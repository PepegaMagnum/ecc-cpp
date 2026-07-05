open_project $PROJ_NAME
 
add_files gf27_ops.cpp
add_files gf163_ops.cpp
 
add_files -tb ../RhoPollard_lib/src/math_operations.cpp \
             -cflags "-I../RhoPollard_lib/include"
 
open_solution "solution1" -flow_target vitis
set_part {xc7z020clg400-1}
create_clock -period 10 -name default
