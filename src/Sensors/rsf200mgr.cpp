#include "rsf200mgr.h"

rsf200mgr::rsf200mgr() {
    mStatus = mIvcam.Init(BOXWORLD_WIDTH, BOXWORLD_HEIGHT,
                        BOXWORLD_WIDTH, BOXWORLD_HEIGHT);

    if (mStatus != SUCCESS)
    {
        fprintf(stderr, "Failed to init ivcam\n");
    }else {
        mFrame.depth = new uint8_t[640*480*2];
        mFrame.color = new uint8_t[640*480*2];

        CalibrationParameters *params = mIvcam.ReadCalibrationData();
    }
}

rsf200mgr::~rsf200mgr() {
    if(mFrame.depth) delete mFrame.depth;
    if(mFrame.color) delete mFrame.color;

    stopCapture();
}

bool rsf200mgr::startCapture() {
    mStatus = mIvcam.StartCapture(IMG_TYPE_COLOR | IMG_TYPE_DEPTH);

    if (mStatus != SUCCESS)
    {
        fprintf(stderr, "Failed to start capturing\n");
        return false;
    }
    return true;
}

uint8_t *rsf200mgr::getDepthBuf()
{
    mStatus = mIvcam.ReadFrame(&mFrame);
    if (mStatus != SUCCESS)
    {
        fprintf(stderr, "Failed to start capturing\n");
    }

    return mFrame.depth;
}

bool rsf200mgr::stopCapture() {
    mStatus = mIvcam.StopCapture();
    return (mStatus == SUCCESS)? true:false;
}
