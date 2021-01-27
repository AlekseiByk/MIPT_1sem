//
// Created by Aleksei on 12.10.2019.
//

#ifndef CPU_ENUM_H
#define CPU_ENUM_H

enum {
#define DEF_CMD(name, num, ASMcode, CPUcode, DIScode)\
    CMD_##name = num,

#include "../commands.h"

#undef DEF_CMD
};

enum {
	REGISTER_VAR = 01,
	NUMBER_VAR = 32,
	MEMORY_VAR = 64,
	REGISTER_ax = 11,
	REGISTER_bx = 12,
	REGISTER_cx = 13,
	REGISTER_dx = 14
};
const int version = 1;
#endif //CPU_ENUM_H
