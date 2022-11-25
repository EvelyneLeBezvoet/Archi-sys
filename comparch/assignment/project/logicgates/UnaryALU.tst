load UnaryALU.hdl,
output-file UnaryALU.out,
compare-to UnaryALU.cmp,
output-list x%B1.16.1 z%B1.1.1 n%B1.1.1 out%B1.16.1;

set x %B0000000000000000;  // x = 0

set z 1,
set n 0,
eval,
output;

set z 0,
set n 0,
eval,
output;

set z 0,
set n 1,
eval,
output;

set x %B1111111111111111;

set z 1,
set n 1,
eval,
output;

set z 0,
set n 1,
eval,
output;

set x %B0000000000010001;

set z 0,
set n 1,
eval,
output;

set x %B1111111111101110;

set z 0,
set n 1,
eval,
output;

set x %B0000000000010001;

set z 1,
set n 1,
eval,
output;
