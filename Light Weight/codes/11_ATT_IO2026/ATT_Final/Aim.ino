bool aimed_before = false;
int aim_cnt = 0;

void aim_to_goal() {
  if (!is_forward || !HS0038.is_ball) {
    aim = 0;
    aim_cnt = 0;
    aimed_before = false;
    return;
  }
  
  if (aimed_before){
    aim_cnt++;
    if(aim_cnt > 30){
      aimed_before = false;
      aim = 0;
      aim_cnt = 0;
    }
    return;
  }

  if (shl > 1700 && !aimed_before) {
    aim = 45;
    aimed_before = true;
    aim_cnt = 0;
    return;
  } 
  if (shr > 1700 && !aimed_before) {
    aim = -45;
    aimed_before = true;
    aim_cnt = 0;
    return;
  } 
  aim = 0;
}
