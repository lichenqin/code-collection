# include <iostream>
# include <ctime>

void Calculate_Angle(){
  time_t rawtime;
  time(&rawtime);

  struct tm* local = localtime(&rawtime);

  int hour = ( local->tm_hour % 12 ) * 5;
  int minute = local->tm_min;

  int Angle_Step = (hour - minuate > 0 ) ? hour - minute : minute - hour;

  std:std::cout << "Angle is: " << Angle_Step*6 << '\n';

}

int main(){
  Calculate_Angle();
  return 0;
}

//the problem is: I didn't consider about offset(Æ«ÒÆÁ¿) about minute hand and hour hand 

// for example: when the time is 1:30:30 
// 		the hour hand will point to 1.5 rather than 1
//		the minute hand will point to 30.5 rather than 30
//		although the result is equal but when we change the time the last result is more accurate
//		maybe i will uploard the next version tommorrow

// first version 24/03/19 00:00 
