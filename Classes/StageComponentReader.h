//
//  StageComponentReader.h
//  DotWar
//
//  Created by ryutamaki on 2015/07/26.
//
//

#ifndef __DotWar__StageComponentReader__
#define __DotWar__StageComponentReader__

#include "cocos2d.h"
#include "cocostudio/WidgetReader/NodeReader/NodeReader.h"

class StageComponentReader : public cocostudio::NodeReader {
public:
    static StageComponentReader* getInstance();
    static void purge();
    cocos2d::Node* createNodeWithFlatBuffers(const flatbuffers::Table* nodeOptions);
};

#endif /* defined(__DotWar__StageButton__) */
