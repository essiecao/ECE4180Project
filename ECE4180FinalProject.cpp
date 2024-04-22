
#include "Thread.h"
#include "mbed.h"
#include "rtos.h"
#include "wave_player.h"
#include "Motor.h"
#include "ultrasonic.h"
#include "Motor.h"
#include "SongPlayer.h"
#include <algorithm>


//Pi mbed USB Slave function
// connect mbed to Pi USB
RawSerial  pi(USBTX, USBRX);

PwmOut speaker(p26);
Motor m(p21, p6, p5); // pwm, fwd, rev , left motor
Motor m2(p22, p8, p7); //right motor
Serial bluetooth(p28, p27); //bluetooth controller

Mutex motor_mutex;
volatile int sonar_distance;
volatile int pi_instruction;

//for motor inputs;
volatile float motor_1_movement;
volatile float motor_2_movement;



 void dist(int distance)
{
    //put code here to execute when the distance has changed
    printf("Distance %d mm\r\n", distance);
}

ultrasonic mu(p11, p12, .1, 1, &dist);


void motor_control(void const *args) {

    while(1) {
    motor_mutex.lock();
    m.speed(motor_1_movement);
    m2.speed(motor_1_movement);
    Thread::wait(20);
    motor_mutex.unlock();
    }

}
void serial_control(void const *agrs) {
    char bnum=0;
    char bhit=0;
    while(1) {
        while (!pi.readable()) {
        Thread::wait(20);
        }
      if (pi.getc()=='!') {
            if (pi.getc()=='B') { //button data packet
                bnum = pi.getc(); //button number
                bhit = pi.getc(); //1=hit, 0=release
               
                    switch (bnum) {

                        case '1': //button 1 will stop the robot
                        if(bhit == '1') {
                            motor_1_movement = 0;
                            motor_2_movement = 0;
                        }

                        break;
                        
                        case '5': //forward, up arrow
                        if (bhit=='1'){      
                        motor_1_movement = -0.5;
                        motor_2_movement = -0.5;
                        }
                            
                            break;
                       
                        case '6': //backward down arrow
                         if (bhit=='1'){
                        motor_1_movement = 0.5;
                        motor_2_movement = 0.5;
                        }
                        
                          
                            break;
                        case '7': //left
                         if (bhit=='1'){
                        motor_1_movement = -0.3;
                        motor_2_movement = 0.3;

                        }

                            break;

                        case '8': //right
                         if (bhit=='1'){
                        motor_1_movement = 0.3;
                        motor_2_movement = -0.3;

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
                        m.speed(-0.5);
                        m2.speed(-0.5);
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
                             m.speed(-0.3);
                             m2.speed(0.3);

                        }

                            break;

                        case '8': //right
                         if (bhit=='1'){
                             m.speed(0.3);
                             m2.speed(-0.3);

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
        int local_sonar_distance;

        // Lock the mutex to safely read sonar_distance
        motor_mutex.lock();
        local_sonar_distance = sonar_distance;
        motor_mutex.unlock();

        // Calculate beep period and duration based on the distance
        float beep_period;
        if (local_sonar_distance > 1000) {
            beep_period = 1.0; // slower beep, 1 second interval
        } else {
            // As distance decreases, increase the frequency of the beeps
            // For example, at 100 mm distance, the period could be very short.
            beep_period = max(0.1,(local_sonar_distance / 1000.0));
        }

        // Set the speaker to beep
        speaker.period(1.0 / 500.0); // Set frequency of the beep
        speaker = 0.5; // Turn on speaker at 50% duty cycle
        Thread::wait(beep_period *200); // Wait for the same period to complete the cycle
        speaker = 0.0; // Turn off speaker
        Thread::wait(beep_period *200);
       
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
        //nihao 
        
    }
 
}
