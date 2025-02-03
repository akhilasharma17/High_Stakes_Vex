#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;
controller Cotton_candy;
brain pancakes;

motor LFdrive(PORT20, ratio18_1);
motor RFdrive(PORT12, ratio18_1, true);
motor LBdrive(PORT19, ratio18_1);
motor RBdrive(PORT11, ratio18_1, true);
motor TopChainMotor(PORT13, ratio18_1, true);
motor GoalMotor(PORT18, ratio18_1, true);
motor FlexMotor(PORT16, ratio18_1, true);
digital_out pistonPort(pancakes.ThreeWirePort.H);

motor_group LeftSide = motor_group (LFdrive, LBdrive);
motor_group RightSide = motor_group (RFdrive, RBdrive);

bool pistonDown = false;
bool armDown = false;

// define your global instances of motors and other devices here

void driveForward( float inches, float distance, float distance2 ) {
        float inchesPerDegree = 25;
        float timer = 0;
        // startRotate doesn't wait for completion
        // this way, the other wheel can turn at same time
        
        while (timer < 30){
        
        LeftSide.spin(vex::forward, inchesPerDegree * distance, vex::percent);
        RightSide.spin(vex::forward, inchesPerDegree * distance, vex::percent);
        timer++;
        }
        vex::task::sleep(distance2);
        LeftSide.stop();
        RightSide.stop();
        RightSide.spin(vex::forward, 0, vex::percent);
        LeftSide.spin(vex::forward,0, vex::percent);
    }
void rotate(float angle){
        if (angle>0){
            int timer=0;
            while (timer < 30){    
                LeftSide.spin(vex::forward, 25, vex::percent);
                RightSide.spin(vex::forward, -25 , vex::percent);
                timer++;
            }
            vex::task::sleep(angle*10.8);
            LeftSide.stop();
            RightSide.stop();
            RightSide.spin(vex::forward, 0, vex::percent);
            LeftSide.spin(vex::forward,0, vex::percent);
        }
        else{
            int timer=0;
            while (timer < 30){    
                LeftSide.spin(vex::forward, -25, vex::percent);
                RightSide.spin(vex::forward, 25 , vex::percent);
                timer++;
            }
            vex::task::sleep(angle*-10.8);
            LeftSide.stop();
            RightSide.stop();
            RightSide.spin(vex::forward, 0, vex::percent);
            LeftSide.spin(vex::forward,0, vex::percent);
        }
    }
void rotateLeft(float angle){
        if (angle>0){
            int timer=0;
            while (timer < 30){    
                LeftSide.spin(vex::forward, 25, vex::percent);
                RightSide.spin(vex::forward, 0 , vex::percent);
                timer++;
            }
            vex::task::sleep(angle*21.6);
            LeftSide.stop();
            RightSide.stop();
            RightSide.spin(vex::forward, 0, vex::percent);
            LeftSide.spin(vex::forward,0, vex::percent);
        }
        else{
            int timer=0;
            while (timer < 30){    
                LeftSide.spin(vex::forward, -25, vex::percent);
                RightSide.spin(vex::forward, 0 , vex::percent);
                timer++;
            }
            vex::task::sleep(angle*-21.6);
            LeftSide.stop();
            RightSide.stop();
            RightSide.spin(vex::forward, 0, vex::percent);
            LeftSide.spin(vex::forward,0, vex::percent);
        }
    }
void PistonToggle(){
    pistonPort.set(!pistonDown);
    pistonDown= !pistonDown; 
}
void ChainForward(float fraction){
    TopChainMotor.spin(forward,110.0,vex::velocityUnits::dps);
    GoalMotor.spin(reverse,110.0,vex::velocityUnits::dps);
    vex::task::sleep(2500*fraction);
    TopChainMotor.setStopping(brakeType::brake);
    TopChainMotor.stop();
}
void ChainReverse(float fraction){
    TopChainMotor.spin(reverse,350.0,vex::velocityUnits::dps);
    GoalMotor.spin(forward,350.0,vex::velocityUnits::dps);
    vex::task::sleep(1500*fraction);
    TopChainMotor.setStopping(brakeType::brake);
    TopChainMotor.stop();
}



void pre_auton(void) {
    Cotton_candy.ButtonA.pressed(PistonToggle);
    pistonPort.set(false);
}

void autonomous(void) {
    ChainForward(1);
    driveForward(1,-100,1375);
    ChainReverse(1.0);
    ChainForward(0.8);
    driveForward(1,100,1375);
}

void usercontrol(void) {
  // User control code here, inside the loop
  //runs the loop over and over throughout the competition
  while (1) {
  int leftSpin = Cotton_candy.Axis3.position(percent);
  int rightSpin = Cotton_candy.Axis2.position(percent);

  if(leftSpin==0){
    LeftSide.setStopping(brakeType::brake);
  }
  if (rightSpin==0)
  {
    RightSide.setStopping(brakeType::brake);
  }

  LeftSide.spin(vex::forward, -leftSpin, vex::percent);
  RightSide.spin(vex::forward, -rightSpin, vex::percent);

  if (Cotton_candy.ButtonR1.pressing()){
    TopChainMotor.spin(forward,110.0,vex::velocityUnits::dps);
  }
  else if (Cotton_candy.ButtonL1.pressing()){
    TopChainMotor.spin(reverse,110.0,vex::velocityUnits::dps);
  }
  else {
    TopChainMotor.setStopping(brakeType::brake);
    TopChainMotor.stop();
  }
  if (Cotton_candy.ButtonL2.pressing()){
    FlexMotor.spin(reverse,110.0,vex::velocityUnits::dps);
  }
  else if (Cotton_candy.ButtonR2.pressing()){
    FlexMotor.spin(forward,110.0,vex::velocityUnits::dps);
  }
  else {
    FlexMotor.setStopping(brakeType::brake);
    FlexMotor.stop();
  }
  if (Cotton_candy.ButtonA.pressing()){
    GoalMotor.spin(forward,110.0,vex::velocityUnits::dps);
  }
  else if (Cotton_candy.ButtonB.pressing()){
    GoalMotor.spin(reverse,110.0,vex::velocityUnits::dps);
  }
  else {
    GoalMotor.setStopping(brakeType::brake);
    GoalMotor.stop();
  }

wait(20, msec); // Sleep the task for a short amount of time to
// prevent wasted resources.
}
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
// Set up callbacks for autonomous and driver control periods.
Competition.autonomous(autonomous);
Competition.drivercontrol(usercontrol);

// Run the pre-autonomous function.
pre_auton();

// Prevent main from exiting with an infinite loop.
while (true) {
wait(100, msec);
}
}
