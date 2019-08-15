/**
* This file is part of ORB-SLAM2.
*
* Copyright (C) 2014-2016 Raúl Mur-Artal <raulmur at unizar dot es> (University of Zaragoza)
* For more information see <https://github.com/raulmur/ORB_SLAM2>
*
* ORB-SLAM2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM2 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.
*/


#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<opencv2/core/core.hpp>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <System.h>
#include <raspicam.h>
#include <raspicam_cv.h>
//#include<unistd>

using namespace std;

void LoadImages(const string &strFile, vector<string> &vstrImageFilenames,
                vector<double> &vTimestamps);

int main(int argc, char **argv)
{
    cv::Mat im;
     cv::String vocabulary="/home/pi/ORB_SLAM2/Vocabulary/ORBvoc.txt";
    cv::String settings="/home/pi/ORB_SLAM2/Settings.yaml";
    raspicam::RaspiCam_Cv Camera;; //Cmaera object
    
     ORB_SLAM2::System SLAM(vocabulary,settings,ORB_SLAM2::System::MONOCULAR,true);
  
    
   for(;;)
   {
       if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;return -1;}
  // cout<<"Camera is open";
     //sleep(1);
    Camera.grab();
   //cout<<"grabed imagefrom rasperry pi";
    Camera.retrieve(im);
        double tframe =0.3503e+01;
        
    cout<<"SLAM started";
    #ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point t1 = std::chrono::monotonic_clock::now();
#endif
    SLAM.TrackMonocular(im,tframe);
    cout<<"Tracked Monocular";
   #ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point t2 = std::chrono::monotonic_clock::now();
#endif

        double ttrack= std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1).count();
        cout<<"Stop camera..."<<endl;
    Camera.release();
}
    
    // Stop all threads
    SLAM.Shutdown();


    // Save camera trajectory
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");

    return 0;
}

void LoadImages(const string &strFile, vector<string> &vstrImageFilenames, vector<double> &vTimestamps)
{
    ifstream f;
    f.open(strFile.c_str());

    // skip first three lines
    string s0;
    getline(f,s0);
    getline(f,s0);
    getline(f,s0);

    while(!f.eof())
    {
        string s;
        getline(f,s);
        if(!s.empty())
        {
            stringstream ss;
            ss << s;
            double t;
            string sRGB;
            ss >> t;
            vTimestamps.push_back(t);
            ss >> sRGB;
            vstrImageFilenames.push_back(sRGB);
        }
    }
}
