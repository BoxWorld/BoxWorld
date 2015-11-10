//
//  ConnBleOSX.m
//  BoxWorld
//
//  Created by Han Maokun on 11/10/15.
//
//
#include "ConnIf.h"

#ifdef __OBJC__
@class ConnBleOSXImp;
#else
typedef struct objc_object ConnBleOSXImp;
#endif

#define NOTIFY_MTU      20

#define BLE_TRANSFER_SERVICE_UUID                   @"E20A39F4-73F5-4BC4-A12F-17D1AD07A961"
#define BLE_TRANSFER_CHARACTERISTIC_NOTIFY_UUID     @"08590F7E-DB05-467E-8757-72F6FAEB13D4"
#define BLE_TRANSFER_CHARACTERISTIC_WRITE_UUID      @"ED6B2379-6A05-497F-9832-F134BF253054"

typedef enum {
    /** RX state: Waiting for initialization / disabled */
    RXSTATE_NONE,
    /** RX state: Synchronizing. */
    RXSTATE_SYNC,
    /** RX state: Receiving frame. */
    RXSTATE_FRAME
} RXSTATE_S;

class ConnBleOSX : public ConnIf
{
public:
    ConnBleOSX(){}
    ConnBleOSX (ConnListenerIf *listener) {
        mConnListener = listener;
        init();
    }
    
    ~ConnBleOSX();
    
    void init();
    void update();
    void sendMsg(Message *msg);
private:
    ConnBleOSXImp* mBleImp;
};
