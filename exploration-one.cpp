#include "common.h"
#include "bump-switch-functions.h"

// Uses functions written above to navigate the Exploration 1 course
void navigateExploration1Course() {

    int QUARTER_POWER_PERCENT = 25;

    DriveSkidFirstUntilHitWall(QUARTER_POWER_PERCENT);
    BackLeftTurnUntilHitWall();
    DriveSkidFirstUntilHitWall(QUARTER_POWER_PERCENT);
    BackRightTurnUntilCornerHitWall();
    DriveSkidFirstUntilHitWall(QUARTER_POWER_PERCENT);
}
