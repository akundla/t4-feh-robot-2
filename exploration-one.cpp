#include "common.h"
#include "bump-switch-functions.h"

// Uses functions written above to navigate the Exploration 1 course
void navigateExploration1Course() {

    int QUARTER_POWER_PERCENT = 25;

    DriveForwardUntilHitWall(QUARTER_POWER_PERCENT, QUARTER_POWER_PERCENT);
    BackLeftTurnUntilHitWall();
    DriveForwardUntilHitWall(QUARTER_POWER_PERCENT, QUARTER_POWER_PERCENT);
    BackRightTurnUntilCornerHitWall();
    DriveForwardUntilHitWall(QUARTER_POWER_PERCENT, QUARTER_POWER_PERCENT);
}
