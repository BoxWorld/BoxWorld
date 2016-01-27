//
//  intelRealsenseMgr.cpp
//  BoxWorld
//
//  Created by Han Maokun on 1/22/16.
//
//
#include "DepthSensorMgr.h"
#include "intelRealsenseMgr.h"
#include "BoxWorldWindowAttrib.h"

intelrsMgr::intelrsMgr() {
    mRSThread = new intelRealsenseThread(this, NULL);
    mBufIdx = 0;
    mDepthData[0] = (uint8_t *)malloc(RESOLUTION_WIDTH * RESOLUTION_HEIGHT * 3);
    mDepthData[1] = (uint8_t *)malloc(RESOLUTION_WIDTH * RESOLUTION_HEIGHT * 3);
    mRSThread->startThread();
}

intelrsMgr::~intelrsMgr() {
    mValid = false;
    if(mRSThread) {
        mRSThread->stopThread();
        mRSThread->waitForThread();
        delete mRSThread;
    }
    if(mDepthData[0]) free(mDepthData[0]);
    if(mDepthData[1]) free(mDepthData[1]);
    stopCapture();
}

void intelrsMgr::stream() {
    rs::context ctx;
    printf("There are %d connected RealSense devices.\n", ctx.get_device_count());
    if(ctx.get_device_count() == 0) {
        mValid = false;
        printf("no realsense hardware connected.\n");
        return;
    }
    
    mDev = ctx.get_device(0);
    
    printf("\nUsing device 0, an %s\n", mDev->get_name());
    printf("    Serial number: %s\n", mDev->get_serial());
    printf("    Firmware version: %s\n", mDev->get_firmware_version());
    
    // Configure depth to run at VGA resolution at 30 frames per second
    mDev->enable_stream(rs::stream::depth, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, rs::format::z16, 30);
    mDev->set_option(rs::option::f200_laser_power, 15);
    mDev->set_option(rs::option::f200_filter_option, 6);
    mDev->set_option(rs::option::f200_accuracy, 3);
    mDev->set_option(rs::option::f200_motion_range, 0);
    mDev->set_option(rs::option::f200_confidence_threshold, 5);
    mDev->start();
    
    // Determine depth value corresponding to one meter
    mOneMeter = static_cast<uint16_t>(1.0f / mDev->get_depth_scale());
    
    mValid = true;
    DepthSensorMgr::getInst().setImp(mOwner);
    
    while(mValid) {
        try{
            uint16_t min_dist, max_dist;
            float dist_range;
            // This call waits until a new coherent set of frames is available on a device
            // Calls to get_frame_data(...) and get_frame_timestamp(...) on a device will return stable values until wait_for_frames(...) is called
            mDev->wait_for_frames();

            min_dist = mOneMeter * (BoxWorldWindowAttrib::getInst().minDist/1000);
            max_dist = mOneMeter * (BoxWorldWindowAttrib::getInst().maxDist/1000);
            dist_range = max_dist - min_dist;
            
            /* Retrieve depth data, which was previously configured as a 
                RESOLUTION_WIDTH x RESOLUTION_HEIGHT image of 16-bit depth values
             */
            const uint16_t *depth_frame = reinterpret_cast<const uint16_t *>(mDev->get_frame_data(rs::stream::depth));
            uint16_t *depth_frame_ptr = (uint16_t *)depth_frame;
            
            uint8_t *cur_buf_p = mDepthData[mBufIdx];
            
            int rowBytes = RESOLUTION_WIDTH * 3;
            for ( int i = 0; i < RESOLUTION_HEIGHT; ++i )
            {
                for ( int j=0; j <RESOLUTION_WIDTH; ++j )
                {
                    uint16_t depth = *depth_frame_ptr++;
                    if(depth > min_dist && depth < max_dist){
                        //uint8_t depth_val = ((float)depth/mOneMeter) * 255;
                        uint8_t depth_val = 255 * ((depth-min_dist)/dist_range);
                        
                        cur_buf_p[i * rowBytes + j*3]   = depth_val;
                        cur_buf_p[i * rowBytes + j*3+1] = 0;
                        cur_buf_p[i * rowBytes + j*3+2] = 0;
                    }else {
                        cur_buf_p[i * rowBytes + j*3]   = 0;
                        cur_buf_p[i * rowBytes + j*3+1] = 0;
                        cur_buf_p[i * rowBytes + j*3+2] = 0;
                    }
                }
            }
            
            mBufIdx += 1; if(mBufIdx == 2) mBufIdx = 0;
        }catch(const rs::error & e){
            // Method calls against librealsense objects may throw exceptions of type rs::error
            printf("rs::error was thrown when calling %s(%s):\n", e.get_failed_function().c_str(), e.get_failed_args().c_str());
            printf("    %s\n", e.what());
            mValid = false;
            return;
        }
    }
    
    mDev->stop();
    printf("realsense thread ended.\n");
    return;
}

void intelrsMgr::startCapture(DepthSensorImp *owner) {
    if(!mValid)
        mRSThread->startThread();

    mOwner = owner;
    /* as realsense thread is already running*/
    printf("start Capture...\n");
}

uint8_t *intelrsMgr::getDepthBuf(){
    return mDepthData[1 - mBufIdx];
}

void intelrsMgr::stopCapture() {
    printf("stop Capture...\n");
}