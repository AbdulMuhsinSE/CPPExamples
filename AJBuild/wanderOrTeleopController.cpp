#include "Aria.h"
#include <pthread.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>

pthread_mutex_t mutex;

ArRobot robot;
void *wanders(void *args)
{


        while(true)
        {
                pthread_mutex_lock(&mutex);
                char command;



                printf("You are now in Wander mode \n");

                ArActionStallRecover recover;
                ArActionBumpers bumpers;
                ArActionAvoidFront avoidFrontNear("Avoid Front Near", 225, 0);
                ArActionAvoidFront avoidFrontFar;
                ArActionConstantVelocity constantVelocity("Constant Velocity", 400);

                robot.addAction(&recover, 100);
                robot.addAction(&bumpers, 75);
                robot.addAction(&avoidFrontNear, 50);
                robot.addAction(&avoidFrontFar, 49);
                robot.addAction(&constantVelocity, 25);
                robot.enableMotors();
                robot.comInt(ArCommands::ENABLE, 1);



                scanf(" %c", &command);
                while(command != 't') {
                        printf("Bad command Sire \n");
                        scanf(" %c", &command);
                }

                robot.stop();
                pthread_mutex_unlock(&mutex);
                usleep(2000);
        }
}

void *teleops(void *args)
{
        while (true)
        {
                pthread_mutex_lock(&mutex);
                printf("You are now in Teleop mode\n");

                char command;
                scanf(" %c", &command);
                while(command != 'w')
                {

                        switch (command) {
                        case ('2'):
                                robot.setRotVel(0);
                                robot.setVel(400);
                                break;
                        case ('8'):
                                robot.setRotVel(0);
                                robot.setVel(-250);
                                break;
                        case ('6'):
                                robot.setRotVel(10);
                                break;
                        case ('4'):
                                robot.setRotVel(-10);
                                break;
                        case ('0'):
                                robot.stop();
                                break;
                        default:
                                printf("Bad Command Sire \n");

                        }

                        robot.enableMotors();
                        robot.comInt(ArCommands::ENABLE, 1);
                        scanf(" %c", &command);
                }
                robot.stop();
                robot.clearDirectMotion();
                pthread_mutex_unlock(&mutex);
                usleep(2000);
        }
}



/** @example demo.cpp General purpose testing and demo program, using ArMode
 *    classes to provide keyboard control of various robot functions.
 *
 *  demo uses ArMode subclasses from ARIA. These modes
 *  provide keyboard control of various aspects and accessories of
 *  the robot, and can be re-used in your programs if you wish.
 *  The ArMode classes are defined in %ArModes.cpp.
 *
 *  "demo" is a useful program for testing out the operation of the robot
 *  for diagnostic or demonstration purposes.  Other example programs
 *  focus on individual areas.
 */

bool handleDebugMessage(ArRobotPacket *pkt)
{
        if(pkt->getID() != ArCommands::MARCDEBUG) return false;
        char msg[256];
        pkt->bufToStr(msg, sizeof(msg));
        msg[255] = 0;
        ArLog::log(ArLog::Terse, "Controller Firmware Debug: %s", msg);
        return true;
}

int main(int argc, char** argv)
{
        Aria::init();
        ArArgumentParser parser(&argc, argv);




        parser.loadDefaultArguments();
        //parser.addDefaultArgument("-rh 10.0.126.20");


        ArRobotConnector robotConnector(&parser, &robot);
        ArAnalogGyro gyro(&robot);

        if (!robotConnector.connectRobot())
        {
                if (!parser.checkHelpAndWarnUnparsed())
                {
                        ArLog::log(ArLog::Terse, "Could not connect to robot, will not have parameter file so options displayed later may not include everything");
                }
                else
                {
                        ArLog::log(ArLog::Terse, "Error, could not connect to robot.");
                        Aria::logOptions();
                        Aria::exit(1);
                }
        }

        if(!robot.isConnected())
        {
                ArLog::log(ArLog::Terse, "Internal error: robot connector succeeded but ArRobot::isConnected() is false!");
        }

        ArLaserConnector laserConnector(&parser, &robot, &robotConnector);

        ArCompassConnector compassConnector(&parser);

        if (!Aria::parseArgs() || !parser.checkHelpAndWarnUnparsed())
        {
                Aria::logOptions();
                Aria::exit(1);
                return 1;
        }

        ArSonarDevice sonarDev;

        robot.addRangeDevice(&sonarDev);

        if (!laserConnector.connectLasers(
                    false, // continue after connection failures
                    false, // add only connected lasers to ArRobot
                    true // add all lasers to ArRobot
                    ))
        {
                printf("Warning: Could not connect to laser(s). Set LaserAutoConnect to false in this robot's individual parameter file to disable laser connection.\n");
        }


        ArTCM2 *compass = compassConnector.create(&robot);
        if(compass && !compass->blockingConnect()) {
                compass = NULL;
        }

        ArUtil::sleep(1000);

        pthread_mutex_init(&mutex, NULL);
        robot.runAsync(true);

        printf("Log: Initializing threads\n");
        pthread_t wanderThread, teleopThread;


        printf("Log: Threads being loaded\n");
        pthread_create(&wanderThread,NULL, wanders,NULL);
        pthread_create(&teleopThread,NULL, teleops,NULL);


        pthread_join(teleopThread, NULL);
        pthread_join(wanderThread, NULL);


        robot.waitForRunExit();


        Aria::exit(0);
        pthread_mutex_destroy(&mutex);
        pthread_exit(NULL);


        return 0;

}
