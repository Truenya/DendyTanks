#pragma once 
#include "../Common/MainProcessor.h"




class IRenderer {
public:
    IRenderer(){};
    virtual void prepare() = 0;
    virtual void processingEventsLoop() = 0;
	virtual ~IRenderer(){};
    virtual bool render() = 0;
	virtual void setProcessor(MainProcessor *processor) = 0;

};
