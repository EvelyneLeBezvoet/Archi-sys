
load CarryLookahead4.hdl,
output-file CarryLookahead4.out,
compare-to CarryLookahead4.cmp,
output-list a%B1.4.1 b%B1.4.1 cin%B1.1.1 out%B1.4.1 cout%B1.1.1;

set a %B0000,
set b %B0000,
set cin %B0,
eval,
output;

set a %B0000,
set b %B0000,
set cin %B1,
eval,
output;

set a %B0000,
set b %B1111,
set cin %B0,
eval,
output;

set a %B1111,
set b %B1111,
set cin %B0,
eval,
output;

set a %B1111,
set b %B1111,
set cin %B1,
eval,
output;

set a %B0101,
set b %B1010,
set cin %B0,
eval,
output;

set a %B1001,
set b %B0001,
set cin %B1,
eval,
output;

set a %B1000,
set b %B1001,
set cin %B1,
eval,
output;

