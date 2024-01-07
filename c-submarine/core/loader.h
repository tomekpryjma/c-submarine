#pragma once

#include <stdio.h>

/*
* Load level file.
* Read level attrs (name, type, how many enemies + their positions, points of interest + positions?)
* $NAME Level name
* $TYPE parent
* $CHILDREN (sub-level1, another-sublevel)
* $DESCRIPTION Lorem ipsum
* $MOBS (456, 34, grunt) ?? - maybe the order should define the order the player will fight these?
* $POIS (345, 567) (340, 670)
*/