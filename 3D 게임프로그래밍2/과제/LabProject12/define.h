#pragma once
#define OBJ_DEAD	1
#define OBJ_NONE	0

#define SAFE_DELETE(p) {delete p; p = nullptr;}