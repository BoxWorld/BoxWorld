//
//  ConnSimuImp.h
//  BoxWorld
//
//  Created by Han Maokun on 10/12/15.
//
//

#ifndef BoxWorld_ConnSimuImp_h
#define BoxWorld_ConnSimuImp_h

#include "ConnIf.h"
#include "ofFileUtils.h"
#include "FileWatcher.h"

typedef enum {
    NORMAL = 0,
    ADDED,
    DELETED
}FILE_EVENTS;

class ConnSimuImp : public ConnIf, FW::FileWatchListener {
public:
    ConnSimuImp (ConnListenerIf *listener) {
        mConnListener = listener;
        init();
    }
    virtual ~ConnSimuImp() {}
    
    void init();
    void handleFileAction(FW::WatchID watchid, const string& dir, const string& filename, FW::Action action);
    void update();
    void sendMsg(Message *msg);

private:
    static void msgCbConstWrapper(void *user_data, int ret_val, void *args);
    void msgCallback(int ret_val, void *args);
    
    FW::FileWatcher *mFileWatcher;
    void onAppAdd(string name);
};

#endif
