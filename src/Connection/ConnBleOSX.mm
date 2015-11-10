//
//  ConnBleOSX.m
//  BoxWorld
//
//  Created by Han Maokun on 11/10/15.
//
//

#import <Cocoa/Cocoa.h>
#import <IOBluetooth/IOBluetooth.h>
#import <Foundation/Foundation.h>
#import "ConnBleOSX.h"
#include "Protocol/slip.h"
#include <queue>
//#include "ConnMgrInst.h"

@interface ConnBleOSXImp : NSObject <CBPeripheralManagerDelegate>

@property (nonatomic, strong) CBPeripheralManager       *peripheralManager;
@property (nonatomic, strong) CBMutableService          *service;
@property (strong, nonatomic) CBMutableCharacteristic   *transferCharacteristic;
@property (strong, nonatomic) CBMutableCharacteristic   *writtenCharacteristic;
@property (atomic) std::queue<Message *>                 sendMsgQ;
@property (atomic) BOOL                                  centralSubscribed;
@property (atomic) Message                              *curSendingMsg;
@property (atomic) NSInteger                             curSendingDataIndex;
@property (atomic) RXSTATE_S                             curRxState;
@property (atomic) NSInteger                             curRxPos;
@property (atomic) BOOL                                  escMode;
@property (nonatomic) unsigned char                     *rxbuf;

-(void)sendMsg:(Message *)msg;

@end

@implementation ConnBleOSXImp

-(id)init
{
    self.centralSubscribed = false;
    self.curRxState = RXSTATE_NONE;
    self.curSendingDataIndex = 0;
    self.peripheralManager = [[CBPeripheralManager alloc] initWithDelegate:self queue:nil];
    self.rxbuf = (unsigned char *)malloc(MAX_RCV_BUF_LEN * sizeof(unsigned char));
    
    return self;
}

/* Build up service with characteristcs and advertise. */
- (void)peripheralManagerDidUpdateState:(CBPeripheralManager *)peripheral
{
    NSLog(@"ConnBleOSXImp@BW$: peripheralManagerDidUpdateState: %d", (int)peripheral.state);
    if (peripheral.state != CBPeripheralManagerStatePoweredOn) {
        return;
    }
    
    // build our service.
    self.writtenCharacteristic =
        [[CBMutableCharacteristic alloc]
                initWithType:[CBUUID UUIDWithString:BLE_TRANSFER_CHARACTERISTIC_WRITE_UUID]
                                         properties:CBCharacteristicPropertyWriteWithoutResponse
                                              value:nil
                                        permissions:CBAttributePermissionsWriteable
        ];
    self.transferCharacteristic =
        [[CBMutableCharacteristic alloc]
                initWithType:[CBUUID UUIDWithString:BLE_TRANSFER_CHARACTERISTIC_NOTIFY_UUID]
                                         properties:CBCharacteristicPropertyNotify
                                              value:nil
                                        permissions:CBAttributePermissionsReadable
        ];
    CBMutableService *transferService =
        [[CBMutableService alloc]
            initWithType:[CBUUID UUIDWithString:BLE_TRANSFER_SERVICE_UUID]
                                        primary:YES
        ];
    transferService.characteristics = @[self.transferCharacteristic, self.writtenCharacteristic];
    
    // And add it to the peripheral manager
    [self.peripheralManager addService:transferService];
    [self.peripheralManager startAdvertising:@{ CBAdvertisementDataServiceUUIDsKey :
                                                        @[[CBUUID UUIDWithString:BLE_TRANSFER_SERVICE_UUID]]
                                              }
    ];
}

/* Catch when ble center subscribes to our characteristic, then ready to sending it data */
- (void)peripheralManager:(CBPeripheralManager *)peripheral central:(CBCentral *)central didSubscribeToCharacteristic:(CBCharacteristic *)characteristic
{
    self.centralSubscribed = true;
}
- (void)peripheralManager:(CBPeripheralManager *)peripheral central:(CBCentral *)central didUnsubscribeFromCharacteristic:(CBCharacteristic *)characteristic
{
    self.centralSubscribed = false;
}

/** This callback comes in when the PeripheralManager is ready to send the next chunk of data.
 *  This is to ensure that packets will arrive in the order they are sent
 */
- (void)peripheralManagerIsReadyToUpdateSubscribers:(CBPeripheralManager *)peripheral
{
    // Start sending again
    [self send];
}

/* remote ble center device has send data. */
- (void)peripheralManager:(CBPeripheralManager *)peripheral didReceiveWriteRequests:(NSArray *)requests
{
    CBATTRequest*       request = [requests  objectAtIndex: 0];
    NSData*             request_data = request.value;
    //NSString *stringFromData = [[NSString alloc] initWithData:request_data encoding:NSUTF8StringEncoding];
    //NSLog(@"ConnBleOSXImp@BW$ Received: %@", stringFromData);
    
    NSUInteger size = [request_data length] / sizeof(unsigned char);
    unsigned char* data_p = (unsigned char*) [request_data bytes];
    NSUInteger data_idx = 0;
    while (data_idx < size) {
        unsigned char data = *(data_p + data_idx);
        data_idx += 1;

        if (_curRxState == RXSTATE_SYNC) {
            if (data == SLIP_END) {
                _curRxState = RXSTATE_FRAME;
                _curRxPos = 0;
            }
        } else if (_curRxState == RXSTATE_FRAME) {
            switch (data) {
                case SLIP_END:
                    if (_curRxPos >= MIN_FRAME_LEN) {
                        //ConnMgrInst::get()->protocolReceiver(_rxbuf, _curRxPos);
                    }
                    _curRxPos = 0;	/* continue in RXSTATE_FRAME */
                    break;
                    
                case SLIP_ESC:
                    _escMode = true;
                    break;
                    
                default:
                    if (_escMode) {
                        _escMode = false;
                        if (data == SLIP_ESC_END)
                            data = SLIP_END;
                        else if (data == SLIP_ESC_ESC)
                            data = SLIP_ESC;
                        else {
                            /* unexpected octet.. resync */
                            _curRxState = RXSTATE_SYNC;
                            return;
                        }
                    }
                    
                    if (_curRxPos >= MAX_RCV_BUF_LEN) {
                        /* Too much data - attempt resync */
                        _curRxState = RXSTATE_SYNC;
                        return;
                    }
                    
                    _rxbuf[_curRxPos++] = data;
            } /* switch (data) */	
        }
    }
}

- (void)peripheralManagerDidStartAdvertising:(CBPeripheralManager *)peripheral error:(NSError *)error{}

- (void)peripheralManager:(CBPeripheralManager *)peripheral didAddService:(CBService *)service error:(NSError *)error{}

- (void)send
{
    if(!_centralSubscribed) return;
    BOOL didSend = YES;
    
    while (didSend) {
        // Work out how big it should be
        NSInteger amountToSend = self.curSendingMsg->getEncodedDataSize() - self.curSendingDataIndex;
        if (amountToSend > NOTIFY_MTU) amountToSend = NOTIFY_MTU;
        
        NSData *chunk = [NSData dataWithBytes:self.curSendingMsg->getEncodedDataP() + self.curSendingDataIndex length:amountToSend];
        
        didSend = [self.peripheralManager updateValue:chunk forCharacteristic:self.transferCharacteristic onSubscribedCentrals:nil];
        
        NSString *stringFromData = [[NSString alloc] initWithData:chunk encoding:NSUTF8StringEncoding];
        
        if (!didSend) {
            return;
        }
        
        NSLog(@"Sent: %@", stringFromData);
        
        self.curSendingDataIndex += amountToSend;
        
        if (self.curSendingDataIndex >= self.curSendingMsg->getEncodedDataSize()) {
            didSend = YES;
            return;
        }
    }
    
    self.sendMsgQ.pop();
    
    if(!self.sendMsgQ.empty()) {
        self.curSendingMsg = self.sendMsgQ.front();
        self.curSendingMsg->encode();
        [self send];
    }
}

- (void)sendMsg:(Message *)msg
{
    if(self.sendMsgQ.empty()) {
        self.sendMsgQ.push(msg);
        self.curSendingMsg = self.sendMsgQ.front();
        self.curSendingMsg->encode();
        [self send];
    }else {
        self.sendMsgQ.push(msg);
    }
}

@end

void ConnBleOSX::init() {
    mBleImp =  [[ConnBleOSXImp alloc] init];
}

ConnBleOSX::~ConnBleOSX() {
    if(mBleImp)
        [mBleImp release];
}

void ConnBleOSX::update() {
    
}

void ConnBleOSX::sendMsg(Message *msg)
{
    [mBleImp sendMsg:msg];
}


