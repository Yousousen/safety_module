////////////////////////////////////////////////////////////////////////////
//
//  This file is part of RTIMULib
//
//  Copyright (c) 2014-2015, richards-tech, LLC
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//  this software and associated documentation files (the "Software"), to deal in
//  the Software without restriction, including without limitation the rights to use,
//  copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
//  Software, and to permit persons to whom the Software is furnished to do so,
//  subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "RTIMULib.h"
#include <cstdio>
#include <math.h>

// yaw = z
// pitch = x
// roll = y

static double inline gforce_to_si(double gforce) {
    return gforce * 9.81;
}

// compensate the accelerometer readings from gravity. 
// @param q the quaternion representing the orientation of a 9DOM MARG sensor array
// @param acc the readings coming from an accelerometer expressed in g
//
// @return a 3d vector representing dinamic acceleration expressed in gravity
void gravity_compensate(RTIMU_DATA& imuData, double* res) {
      double g[] = {0.0, 0.0, 0.0};
      const double x = imuData.accel.x();
      const double y = imuData.accel.y();
      const double z = imuData.accel.z();
      double acc[3]={x,y,z}; // Data will come from the accelerometers.

      const double scalar = imuData.fusionQPose.scalar();
      const double qx = imuData.fusionQPose.x();
      const double qy = imuData.fusionQPose.y();
      const double qz = imuData.fusionQPose.z();

      double q[4] = {scalar, qy, qy, qz};
        
        // Get expected direction of g
        g[0] = 2 * (q[1] * q[3] - q[0] * q[2]);
        g[1] = 2 * (q[0] * q[1] + q[2] * q[3]);
        g[2] = q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];
        

        // Compensate accelerometer readings with the expected direction of g
        res[0] = acc[0] - g[0];
        res[1] = acc[1] - g[1];
        res[2] = acc[2] - g[2];
}


int main() {
    //  Using custom RTIMULib.ini with calibrated sensors.
    RTIMUSettings *settings = new RTIMUSettings("./RTIMULib.ini");

    RTIMU *imu = RTIMU::createIMU(settings);

    if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL)) {
        printf("No IMU found\n");
        exit(1);
    }

    //  This is an opportunity to manually override any settings before the
    //  call IMUInit

    //  Set up IMU
    imu->IMUInit();

    //  This is a convenient place to change fusion parameters
    imu->setSlerpPower(0.02);
    imu->setGyroEnable(true);
    imu->setAccelEnable(true);
    imu->setCompassEnable(true);

    //  now just process data
    while (1) {
        //  poll at the rate recommended by the IMU
        usleep(imu->IMUGetPollInterval() * 1000);
        while (imu->IMURead()) {
            RTIMU_DATA imuData = imu->getIMUData();

            double earth_acc[3] = {0.0, 0.0, 0.0};
            gravity_compensate(imuData, earth_acc);

            printf("\n");

            printf("ACCELERATION IN M/S^-2\n");
            printf("Acceleration        X: %.2f\n",
                    gforce_to_si(imuData.accel.x()));
            printf("Acceleration        Y: %.2f\n",
                    gforce_to_si(imuData.accel.y()));
            printf("Acceleration (no g) Z: %.2f\n",
                    gforce_to_si(imuData.accel.z() - 1));

            printf("GRAVITY COMP IN M/S^-2\n");
            printf("Gravity Compensated X: %.2f\n", gforce_to_si(earth_acc[0]));
            printf("Gravity Compensated Y: %.2f\n", gforce_to_si(earth_acc[1]));
            printf("Gravity Compensated Z: %.2f\n", gforce_to_si(earth_acc[2]));

            printf("FUSION POSE\n");
            printf("%-15s%-s\n", "degrees fusionpose",
                    RTMath::displayDegrees("", imuData.fusionPose));
            printf("x in degrees: %.2f\n", imuData.fusionPose.x() *
                    RTMATH_RAD_TO_DEGREE);
            printf("y in degrees: %.2f\n", imuData.fusionPose.y() *
                    RTMATH_RAD_TO_DEGREE);
            printf("z in degrees: %.2f\n", imuData.fusionPose.z() *
                    RTMATH_RAD_TO_DEGREE);
            fflush(stdout);
        }
    }
}
