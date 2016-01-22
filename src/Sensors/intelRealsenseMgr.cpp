//
//  intelRealsenseMgr.cpp
//  BoxWorld
//
//  Created by Han Maokun on 1/22/16.
//
//
#include "DepthSensorMgr.h"
#include "intelRealsenseMgr.h"

intelrsMgr::intelrsMgr() {
    mRSThread = new intelRealsenseThread(this, NULL);
    mBufIdx = 0;
    mDepthData[0] = (uint8_t *)malloc(640 * 480 * 3);
    mDepthData[1] = (uint8_t *)malloc(640 * 480 * 3);
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
    mDev->enable_stream(rs::stream::depth, 640, 480, rs::format::z16, 30);
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
            // This call waits until a new coherent set of frames is available on a device
            // Calls to get_frame_data(...) and get_frame_timestamp(...) on a device will return stable values until wait_for_frames(...) is called
            mDev->wait_for_frames();

            // Retrieve depth data, which was previously configured as a 640 x 480 image of 16-bit depth values
            const uint16_t *depth_frame = reinterpret_cast<const uint16_t *>(mDev->get_frame_data(rs::stream::depth));
            uint16_t *depth_frame_ptr = (uint16_t *)depth_frame;
            
            uint8_t *cur_buf_p = mDepthData[mBufIdx];
            
            int rowBytes = 640 * 3;
            for ( int i = 0; i < 480; ++i )
            {
                for ( int j=0; j <640; ++j )
                {
                    uint16_t depth = *depth_frame_ptr++;
                    if(depth > 0 && depth < mOneMeter){
                        uint8_t depth_val = ((float)depth/mOneMeter) * 255;
                        
                        cur_buf_p[i * rowBytes + j*3]   = depth_val;
                        cur_buf_p[i * rowBytes + j*3+1] = depth_val;
                        cur_buf_p[i * rowBytes + j*3+2] = depth_val;
                    }else {
                        cur_buf_p[i * rowBytes + j*3]   = 1;
                        cur_buf_p[i * rowBytes + j*3+1] = 1;
                        cur_buf_p[i * rowBytes + j*3+2] = 1;
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