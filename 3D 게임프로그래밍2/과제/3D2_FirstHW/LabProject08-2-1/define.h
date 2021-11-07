#pragma once

#define MAXBULLETNUM 1000
#define BUILDINGNUM 100
#define EXPLOSION_NUM 50
#define ATTACKCOOLTIME 0.3f

namespace OBJ {
	enum OBJ_INDEX {
		BULLET, BUILDING, END
	};
}


namespace SHADER {
	enum SHADERS_INDEX {
		OBJECTS, BILLBOARD, END
	};
}
