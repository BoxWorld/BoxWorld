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
#include "SimuDepthmapModel.h"

intelrsMgr::intelrsMgr() {
    mRSThread = new intelRealsenseThread(this, NULL);
    mBufIdx = 0;
    mNumPixels = RESOLUTION_WIDTH * RESOLUTION_HEIGHT;
    mDepthFloatImage[0].allocate(RESOLUTION_WIDTH, RESOLUTION_HEIGHT,OF_IMAGE_GRAYSCALE);
    mDepthFloatImage[1].allocate(RESOLUTION_WIDTH, RESOLUTION_HEIGHT,OF_IMAGE_GRAYSCALE);
    
    mDepthFloatPixels[0] = mDepthFloatImage[0].getPixels();
    mDepthFloatPixels[1] = mDepthFloatImage[1].getPixels();
    for(int i=0; i<mNumPixels; i++) {
        mDepthFloatPixels[0][i] = 1.0;
        mDepthFloatPixels[1][i] = 1.0;
    }
    
    mRSThread->startThread();
}

intelrsMgr::~intelrsMgr() {
    mValid = false;
    if(mRSThread) {
        mRSThread->stopThread();
        mRSThread->waitForThread();
        delete mRSThread;
    }
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
    mDev->enable_stream(rs::stream::depth, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, rs::format::z16, 15);
    //mDev->enable_stream(rs::stream::color, rs::preset::best_quality);
    //mDev->enable_stream(rs::stream::infrared, rs::preset::best_quality);
    
    mDev->set_option(rs::option::f200_laser_power, 15);
    mDev->set_option(rs::option::f200_filter_option, 6);
    mDev->set_option(rs::option::f200_accuracy, 3);
    mDev->set_option(rs::option::f200_motion_range, 100);
    mDev->set_option(rs::option::f200_confidence_threshold, 5);
    mDev->start();
    
    // Determine depth value corresponding to one meter
    mOneMeter = (1.0f / mDev->get_depth_scale());
    
    mValid = true;
    DepthSensorMgr::getInst().setImp(mOwner);
    mBufIdx = 0;
    
    while(mValid) {
        try{
            float min_dist, max_dist;
            // This call waits until a new coherent set of frames is available on a device
            // Calls to get_frame_data(...) and get_frame_timestamp(...) on a device will return stable values until wait_for_frames(...) is called
            mDev->wait_for_frames();
            
            mDepthFloatPixels[mBufIdx] = mDepthFloatImage[mBufIdx].getPixels();

            min_dist = mOneMeter * (BoxWorldWindowAttrib::getInst().minDist/1000.0);
            max_dist = mOneMeter * (BoxWorldWindowAttrib::getInst().maxDist/1000.0);
            
            /* Retrieve depth data, which was previously configured as a 
                RESOLUTION_WIDTH x RESOLUTION_HEIGHT image of 16-bit depth values
             */
            const uint16_t *depth_frame = reinterpret_cast<const uint16_t *>(mDev->get_frame_data(rs::stream::depth));
            uint16_t *depth_frame_ptr;
            
            for ( int i = RESOLUTION_HEIGHT-1; i > 0; i-- ){
                for ( int j=0; j<RESOLUTION_WIDTH; j++ ){
                    uint32_t row_idx = RESOLUTION_HEIGHT - i;
                    uint32_t s_idx = i * RESOLUTION_WIDTH + j;
                    uint32_t t_idx = row_idx * RESOLUTION_WIDTH + j;
                    depth_frame_ptr = (uint16_t *)depth_frame + s_idx;
                    
                    if(*depth_frame_ptr > min_dist && *depth_frame_ptr < max_dist){
                        mDepthFloatPixels[mBufIdx][t_idx] = ofMap(*depth_frame_ptr, max_dist, min_dist, 0.0f, 1.0f);
                    }else if(*depth_frame_ptr <= min_dist) {
                        mDepthFloatPixels[mBufIdx][t_idx] = 1.0;
                    }else if(*depth_frame_ptr >= max_dist) {
                        mDepthFloatPixels[mBufIdx][t_idx] = 0.0;
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
    
    mDev->disable_stream(rs::stream::depth);
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

ofTexture & intelrsMgr::getDepthTexture(){
    int idx = mBufIdx + 1; if(idx == 2) idx = 0;
    mDepthFloatImage[idx].setFromPixels(mDepthFloatPixels[idx],
                                            RESOLUTION_WIDTH,
                                            RESOLUTION_HEIGHT,
                                            OF_IMAGE_GRAYSCALE
                                        );
    
    return DepthTransMgr::get()->getTransTexture(mDepthFloatImage[mBufIdx].getTexture(),BoxWorldWindowAttrib::getInst().minDist, BoxWorldWindowAttrib::getInst().maxDist);
    //return mDepthFloatImage[mBufIdx].getTexture();
}

void intelrsMgr::stopCapture() {
    printf("stop Capture...\n");
}

void intelrsMgr::save() {
    int idx = mBufIdx + 1; if(idx == 2) idx = 0;
    mDepthFloatImage[idx].setFromPixels(mDepthFloatPixels[idx],
                                            RESOLUTION_WIDTH,
                                            RESOLUTION_HEIGHT,
                                            OF_IMAGE_GRAYSCALE
                                        );
    mDepthFloatImage[idx].saveImage("sample.png");
}
