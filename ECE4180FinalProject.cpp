#include "Thread.h"
#include "mbed.h"
#include "rtos.h"
#include "wave_player.h"
#include "Motor.h"
#include "ultrasonic.h"
#include "Motor.h"
#include "SongPlayer.h"


PwmOut speaker(p26);
Motor m(p21, p6, p5); // pwm, fwd, rev , left motor
Motor m2(p22, p8, p7); //right motor
Serial bluetooth(p28, p27); //bluetooth controller

Mutex motor_mutex;
volatile int sonar_distance;



 void dist(int distance)
{
    //put code here to execute when the distance has changed
    printf("Distance %d mm\r\n", distance);
}

ultrasonic mu(p11, p12, .1, 1, &dist);


void motor_control(void const *args) {

    while(1) {
    // for (float s= -1.0; s < 1.0 ; s += 0.01) {
    //    m.speed(s); 
    //    m2.speed(s);
    //    Thread::wait(20);
    // }
    }

}
void serial_control(void const *agrs) {

}
void bluetooth_control(void const *args) {
     char bnum=0;
    char bhit=0;
    while(1) {
        while (!bluetooth.readable()) {
            Thread::wait(20);
        }
       // four_slots.wait();
        motor_mutex.lock();
        if (bluetooth.getc()=='!') {
            if (bluetooth.getc()=='B') { //button data packet
                bnum = bluetooth.getc(); //button number
                bhit = bluetooth.getc(); //1=hit, 0=release
               
                    switch (bnum) {

                        case '1': //button 1 will stop the robot
                        if(bhit == '1') {
                            m.speed(0);
                            m2.speed(0);
                        }

                        break;
                        
                        case '5': //forward, up arrow
                        if (bhit=='1'){      
                        m.speed(-0.2);
                        m2.speed(-0.2);
                        }
                            
                            break;
                       
                        case '6': //backward down arrow
                         if (bhit=='1'){
                        m.speed(0.5);
                        m2.speed(0.5);
                        }
                        
                          
                            break;
                        case '7': //left
                         if (bhit=='1'){
                             m.speed(0);
                             m2.speed(0.2);

                        }

                            break;

                        case '8': //right
                         if (bhit=='1'){
                             m.speed(0.2);
                             m2.speed(0);

                        }

                            break;
                        
                        default:
                            break;
                    
                }
            }
        }
        motor_mutex.unlock();
    }

}
void sonar_control(void const *args) {
    while(1){
        mu.checkDistance();
        sonar_distance = mu.getCurrentDistance();
    }

}


void speaker_control(void const *args) {
    while(1) {
    // speaker.period(1.0/500.0); // 500hz period
    //    speaker = 0.5;
    //    Thread::wait(2000);
    //    speaker = 0.0;
    //    Thread::wait(2000);
    }
}




int main()
{
    
    mu.startUpdates();//start measuring the distance
  


    Thread thread2(motor_control);
    Thread thread3(serial_control);
    Thread thread4(bluetooth_control);
    Thread thread5(sonar_control);
    Thread thread6(speaker_control);

    while(1) {
        //do stuff
        
    }
 
}
