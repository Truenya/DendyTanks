#pragma once 
#include "AbstractRenderer.h"
#include "../Common/MainProcessor.h"
#include <iostream>
#include <linux/input.h>
#include <ncurses.h>
#include "FPS.h"

const wchar_t STAR = 0X272F; 
class CRenderer : public IRenderer{
public:
    enum class Shift {
        INCREMENT,
		DECREMENT,
    };
    CRenderer(std::atomic_bool &running);
    virtual void prepare();
    virtual void processingEventsLoop();
	virtual ~CRenderer(){};
    virtual bool render();
    virtual void setProcessor(MainProcessor *processor); 
    void processEvents(){};
private:          
    std::atomic_bool &work_;
	MainProcessor *processor_;
    void init();
	bool load();
    frame_rater* obj;
};