#include "ofxControlPanel.h"

//----------------------------
ofxControlPanel::ofxControlPanel(){
    dragging        = false;
    minimize        = false;
    selectedPanel   = 0;
    currentPanel    = 0;
    bUseTTFFont     = false;
    usingXml        = false;
    saveDown        = false;
    restoreDown     = false;
    incrementSave   = false;
    hidden          = false;
    bDraggable      = true;

    currentXmlFile = "";
    incrementSaveName = "";
    xmlObjects.clear();
	
}

ofxControlPanel::~ofxControlPanel(){
    for(int i = 0; i < guiObjects.size(); i++){
        delete guiObjects[i];
    }
}

//-----------------------------
void ofxControlPanel::setup(string controlPanelName, float panelX, float panelY, float width, float height){
      name = controlPanelName;

      setPosition(panelX, panelY);
      setDimensions(width, height);
      setShowText(true);
}

//-----------------------------
void ofxControlPanel::loadFont( string fontName, int fontsize ){
    guiTTFFont.loadFont(fontName, fontsize);
    bool okay = guiTTFFont.bLoadedOk;

    if(okay){
        bUseTTFFont = true;
        for(int i = 0; i < guiObjects.size(); i++){
            guiObjects[i]->setFont(&guiTTFFont);
        }
    }
}

//---------------------------------------------
guiTypePanel * ofxControlPanel::addPanel(string panelName, int numColumns, bool locked){
    guiTypePanel * panelPtr = new guiTypePanel();
    panelPtr->setup(panelName);
    panelPtr->setPosition(borderWidth, topSpacing);
    panelPtr->setDimensions(boundingBox.width - borderWidth*2, boundingBox.height - topSpacing*3);
    if( locked )panelPtr->lock();
    else panelPtr->unlock();

    if( numColumns > 1 ){
        for(int i = 1; i < numColumns; i++){
            panelPtr->addColumn(30);
        }
    }

    panels.push_back(panelPtr);
    panelTabs.push_back(ofRectangle());

    guiObjects.push_back(panelPtr);
    if( bUseTTFFont ){
        panelPtr->setFont(&guiTTFFont);
    }

    return panelPtr;
}

// ############################################################## //
// ##
// ##      Control where things are added to
// ##
// ############################################################## //


//---------------------------------------------
void ofxControlPanel::setWhichPanel(int whichPanel){
    if( whichPanel < 0 || whichPanel >= panels.size() )return;
    currentPanel = whichPanel;
    setWhichColumn(0);
}

//---------------------------------------------
void ofxControlPanel::setWhichPanel(string panelName){
    for(int i = 0; i < panels.size(); i++){
        if( panels[i]->name == panelName){
            setWhichPanel(i);
            setWhichColumn(0);
            return;
        }
    }
}

//---------------------------------------------
void ofxControlPanel::setWhichColumn(int column){
    if( currentPanel < 0 || currentPanel >= panels.size() )return;
    panels[currentPanel]->selectColumn(column);
}


// ############################################################## //
// ##
// ##       Gui elements
// ##
// ############################################################## //

//-------------------------------
void ofxControlPanel::setSliderWidth(int width){

}

//---------------------------------------------
guiTypeToggle * ofxControlPanel::addToggle(string name, string xmlName, bool defaultValue){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;

    //add a new slider to our list
    guiTypeToggle * tmp = new guiTypeToggle();

    //setup and dimensions
    tmp->setup(name, (bool)defaultValue);
    tmp->setDimensions(14, 14);
    tmp->setTypeBool();
    tmp->xmlName = xmlName;

    xmlObjects.push_back( xmlAssociation(tmp, xmlName, 1) );
    panels[currentPanel]->addElement( tmp );

    guiObjects.push_back(tmp);
    if( bUseTTFFont ){
        tmp->setFont(&guiTTFFont);
    }

    return tmp;
}


//---------------------------------------------
guiTypeMultiToggle * ofxControlPanel::addMultiToggle(string name, string xmlName, int defaultBox, vector <string> boxNames){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;

    //add a new multi toggle to our list
    guiTypeMultiToggle * tmp = new guiTypeMultiToggle();

    //setup and dimensions
    tmp->setup(name, defaultBox, boxNames);
    tmp->setDimensions(180, 60);
    tmp->xmlName = xmlName;

    //we can say we want to an int or a float!
    tmp->setTypeInt();

    xmlObjects.push_back( xmlAssociation(tmp, xmlName, 1) );
    guiObjects.push_back(tmp);

    if( bUseTTFFont ){
        tmp->setFont(&guiTTFFont);
    }

    panels[currentPanel]->addElement( tmp );

    return tmp;
}

//-------------------------------
guiTypeSlider * ofxControlPanel::addSlider(string sliderName, string xmlName, float value , float min, float max, bool isInt){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;

    //add a new slider to our list
    guiTypeSlider * tmp = new guiTypeSlider();

    //setup and dimensions
    tmp->setup(sliderName, value, min, max);
    tmp->setDimensions(180, 10);
    tmp->xmlName = xmlName;

    //we can say we want to an int or a float!
    if(isInt){
        tmp->setTypeInt();
    }else{
        tmp->setTypeFloat();
    }

    xmlObjects.push_back( xmlAssociation(tmp, xmlName, 1) );
    guiObjects.push_back(tmp);
    if( bUseTTFFont ){
        tmp->setFont(&guiTTFFont);
    }

    panels[currentPanel]->addElement( tmp );

    return tmp;
}

//-------------------------------
guiType2DSlider * ofxControlPanel::addSlider2D(string sliderName, string xmlName, float valueX, float valueY, float minX, float maxX, float minY, float maxY, bool isInt){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;

    //add a new slider to our list
    guiType2DSlider * tmp = new guiType2DSlider();

    //setup and dimensions
    tmp->setup(sliderName, valueX, minX, maxX, valueY, minY, maxY);
    tmp->setDimensions(180, 80);
    tmp->xmlName = xmlName;

    //we can say we want to an int or a float!
    if(isInt){
        tmp->setTypeInt();
    }else{
        tmp->setTypeFloat();
    }

    xmlObjects.push_back( xmlAssociation(tmp, xmlName, 2) );
    guiObjects.push_back(tmp);

    if( bUseTTFFont ){
        tmp->setFont(&guiTTFFont);
    }

    panels[currentPanel]->addElement( tmp );

    return tmp;
}



//---------------------------------------------
guiTypeDrawable * ofxControlPanel::addDrawableRect(string name, ofBaseDraws * drawablePtr, int drawW, int drawH){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;
    guiTypeDrawable * vid = new guiTypeDrawable();

    vid->setup(name, drawablePtr, drawW, drawH);
    panels[currentPanel]->addElement(vid);

    guiObjects.push_back(vid);
    if( bUseTTFFont ){
        vid->setFont(&guiTTFFont);
    }


    return vid;
}

//---------------------------------------------
guiTypeCustom * ofxControlPanel::addCustomRect(string name, guiCustomImpl * customPtr, int drawW, int drawH){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;
    guiTypeCustom * custom = new guiTypeCustom();

    custom->setup(name, customPtr, drawW, drawH);
    panels[currentPanel]->addElement(custom);
    guiObjects.push_back(custom);

    if( bUseTTFFont ){
        custom->setFont(&guiTTFFont);
    }

    return custom;
}

//---------------------------------------------
guiTypeLogger * ofxControlPanel::addLogger(string name, simpleLogger * logger, int drawW, int drawH){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;
    guiTypeLogger * loggerType = new guiTypeLogger();

    loggerType->setup(name, logger, drawW, drawH);
    panels[currentPanel]->addElement(loggerType);

    guiObjects.push_back(loggerType);

    if( bUseTTFFont ){
        loggerType->setFont(&guiTTFFont);
    }

    return loggerType;
}

//---------------------------------------------
guiTypeFileLister * ofxControlPanel::addFileLister(string name, simpleFileLister * lister, int drawW, int drawH){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;
    guiTypeFileLister * listerType = new guiTypeFileLister();

    listerType->setup(name, lister, drawW, drawH);
    panels[currentPanel]->addElement(listerType);

    guiObjects.push_back(listerType);

    if( bUseTTFFont ){
        listerType->setFont(&guiTTFFont);
    }

    return listerType;
}

// ############################################################## //
// ##
// ##       get and set values
// ##
// ############################################################## //

//---------------------------------------------
void ofxControlPanel::setValueB(string xmlName, bool value,  int whichParam){
    for(int i = 0; i < guiObjects.size(); i++){
        if( guiObjects[i]->xmlName == xmlName){
            if( whichParam >= 0  ){
                guiObjects[i]->value.setValue(value, whichParam);
                return;
            }
        }
    }
}

//---------------------------------------------
void ofxControlPanel::setValueI(string xmlName, int value,  int whichParam){
    for(int i = 0; i < guiObjects.size(); i++){
        if( guiObjects[i]->xmlName == xmlName){
            if( whichParam >= 0  ){
                guiObjects[i]->value.setValue(value, whichParam);
                return;
            }
        }
    }
}

//---------------------------------------------
void ofxControlPanel::setValueF(string xmlName, float value,  int whichParam){
    for(int i = 0; i < guiObjects.size(); i++){
        if( guiObjects[i]->xmlName == xmlName){
            if( whichParam >= 0  ){
                guiObjects[i]->value.setValue(value, whichParam);
                return;
            }
        }
    }
}


//---------------------------------------------
bool ofxControlPanel::getValueB(string xmlName, int whichParam){
    for(int i = 0; i < xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj != NULL && xmlName == xmlObjects[i].xmlName ){
            if( whichParam >= 0 && whichParam < xmlObjects[i].numParams ){
                return xmlObjects[i].guiObj->value.getValueB(whichParam);
            }
        }
    }
    ofLog(OF_LOG_WARNING, "ofxControlPanel - paramater requested %s doesn't exist - returning 0", xmlName.c_str());
    return 0;
}

//---------------------------------------------
float ofxControlPanel::getValueF(string xmlName, int whichParam){
    for(int i = 0; i < xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj != NULL && xmlName == xmlObjects[i].xmlName ){
            if( whichParam >= 0 && whichParam < xmlObjects[i].numParams ){
                return xmlObjects[i].guiObj->value.getValueF(whichParam);
            }
        }
    }
    ofLog(OF_LOG_WARNING, "ofxControlPanel - paramater requested %s doesn't exist - returning 0", xmlName.c_str());
    return 0;
}

//---------------------------------------------
int ofxControlPanel::getValueI(string xmlName, int whichParam){
    for(int i = 0; i < xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj != NULL && xmlName == xmlObjects[i].xmlName ){
            if( whichParam >= 0 && whichParam < xmlObjects[i].numParams ){
                return xmlObjects[i].guiObj->value.getValueI(whichParam);
            }
        }
    }
    ofLog(OF_LOG_WARNING, "ofxControlPanel - paramater requested %s doesn't exist - returning 0", xmlName.c_str());
    return 0;
}


// ############################################################## //
// ##
// ##       Settings to/from xml
// ##
// ############################################################## //

//-----------------------------
void ofxControlPanel::setIncrementSave(string incrmentalFileBaseName){
    incrementSaveName = incrmentalFileBaseName;
    incrementSave = true;
}
//-----------------------------
void ofxControlPanel::disableIncrementSave(){
    incrementSave = false;
}

//-----------------------------
void ofxControlPanel::loadSettings(string xmlFile){
    for(int i = 0; i < guiObjects.size(); i++)guiObjects[i]->loadSettings(xmlFile);

    currentXmlFile = xmlFile;

    settingsDirectory = currentXmlFile;

    int posLastSlash = settingsDirectory.rfind("/");
    if( posLastSlash > 0) settingsDirectory.erase(settingsDirectory.begin()+ posLastSlash+1, settingsDirectory.end()  );
    else settingsDirectory = "";

    settings.loadFile(currentXmlFile);
    usingXml = true;

    for(int i = 0; i < xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj != NULL ){
            int numParams = xmlObjects[i].numParams;

            for(int j = 0; j < numParams; j++){
                string str = xmlObjects[i].xmlName+":val_"+ofToString(j);
                float val = settings.getValue(str, xmlObjects[i].guiObj->value.getValueF(j));

                xmlObjects[i].guiObj->value.setValue(val, j);
            }
        }
    }
}

//-----------------------------
void ofxControlPanel::reloadSettings(){
    for(int i = 0; i < guiObjects.size(); i++)guiObjects[i]->reloadSettings();

    if( currentXmlFile != "" ){

        settings.loadFile(currentXmlFile);
        usingXml = true;

        for(int i = 0; i < xmlObjects.size(); i++){
            if( xmlObjects[i].guiObj != NULL ){
                int numParams = xmlObjects[i].numParams;

                for(int j = 0; j < numParams; j++){
                    string str = xmlObjects[i].xmlName+":val_"+ofToString(j);
                    float val = settings.getValue(str, xmlObjects[i].guiObj->value.getValueF(j));

                    xmlObjects[i].guiObj->value.setValue(val, j);
                }
            }
        }

    }
}

//-------------------------------
void ofxControlPanel::saveSettings(string xmlFile){
    for(int i = 0; i < guiObjects.size(); i++)guiObjects[i]->saveSettings(xmlFile);

    for(int i = 0; i < xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj != NULL ){
            int numParams = xmlObjects[i].numParams;

            for(int j = 0; j < numParams; j++){
                string str = xmlObjects[i].xmlName+":val_"+ofToString(j);
                settings.setValue(str, xmlObjects[i].guiObj->value.getValueF(j));
            }
        }
    }

    if( incrementSave ){
        string xmlName = incrementSaveName;
        xmlName += ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
        xmlName += ".xml";
        xmlName = settingsDirectory + xmlName;

        settings.saveFile(xmlName);
    }
    settings.saveFile(xmlFile);
    currentXmlFile = xmlFile;
    usingXml = true;
}

//-----------------------------
void ofxControlPanel::saveSettings(){
    for(int i = 0; i < guiObjects.size(); i++)guiObjects[i]->saveSettings();

    for(int i = 0; i < xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj != NULL ){
            int numParams = xmlObjects[i].numParams;

            for(int j = 0; j < numParams; j++){
                string str = xmlObjects[i].xmlName+":val_"+ofToString(j);
                settings.setValue(str, xmlObjects[i].guiObj->value.getValueF(j));
            }
        }
    }
    if( incrementSave ){
        string xmlName = incrementSaveName;
        xmlName += ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
        xmlName += ".xml";
        xmlName = settingsDirectory + xmlName;

        settings.saveFile(xmlName);
    }
    settings.saveFile(currentXmlFile);
    usingXml = true;
}

// ############################################################## //
// ##
// ##       Visibilty
// ##
// ############################################################## //


//---------------------------
void ofxControlPanel::setMinimized(bool bMinimize){
    minimize = bMinimize;
}

//---------------------------
void ofxControlPanel::setDraggable(bool bDrag){
    bDraggable = bDrag;
}

//-------------------------------
void ofxControlPanel::show(){
    hidden = false;
}

//-------------------------------
void ofxControlPanel::hide(){
    hidden = true;
}

//-------------------------------
void ofxControlPanel::toggleView(){
    hidden = !hidden;
}

// ############################################################## //
// ##
// ##       Mouse Events
// ##
// ############################################################## //


//-------------------------------
void ofxControlPanel::mousePressed(float x, float y, int button){
    if( hidden ) return;

    bool tabButtonPressed = false;

    if( isInsideRect(x, y, minimizeButton)){
        minimize = !minimize;
    }else if( usingXml && isInsideRect(x, y, saveButton) ){
        saveSettings();
        saveDown = true;
    }else if( usingXml && isInsideRect(x, y, restoreButton) ){
        reloadSettings();
        restoreDown = true;
    }else if( isInsideRect(x, y, topBar) && bDraggable){
        dragging = true;
        mouseDownPoint.set(x - boundingBox.x, y-boundingBox.y, 0);
    }else if(!minimize){
        for(int i = 0; i < panels.size(); i++){
            if( isInsideRect(x, y, panelTabs[i]) ){
                selectedPanel = i;
                tabButtonPressed = true;
                break;
            }
        }
    }

    if(minimize == false && tabButtonPressed == false && isInsideRect(x, y, boundingBox) ){
        for(int i = 0; i < panels.size(); i++){
             if( i == selectedPanel )panels[i]->checkHit( x - hitArea.x, y - hitArea.y, button);
        }
    }

    prevMouse.set(x, y);
}


//-------------------------------
void ofxControlPanel::mouseDragged(float x, float y, int button){
    if( hidden ) return;

    if(dragging)setPosition( MAX(0, x - mouseDownPoint.x), MAX(0, y -mouseDownPoint.y));
    else if(!minimize){
        for(int i = 0; i < panels.size(); i++){
            if( i == selectedPanel ){

                if(button){
                    panels[i]->updateGui( x - prevMouse.x, y - prevMouse.y, false, true);
                }else{
                    panels[i]->updateGui( x - hitArea.x, y - hitArea.y, false, false);
                }
            }
        }
    }

    prevMouse.set(x, y);
}

//-------------------------------
void ofxControlPanel::mouseReleased(){
    if( hidden ) return;

    for(int i = 0; i < panels.size(); i++){
        panels[i]->release();
    }
    dragging        = false;
    saveDown        = false;
    restoreDown     = false;
}


// ############################################################## //
// ##
// ##       Updater
// ##
// ############################################################## //

//-------------------------------
void ofxControlPanel::update(){
    guiBaseObject::update();

    topBar           = ofRectangle(boundingBox.x, boundingBox.y, boundingBox.width, 20);
    minimizeButton   = ofRectangle(boundingBox.x + boundingBox.width - 24, boundingBox.y + 4, 20, 10 );
    saveButton       = ofRectangle(boundingBox.x + displayText.getTextWidth() + 20, boundingBox.y + 4, 40, 12 );
    restoreButton    = ofRectangle(saveButton.x + saveButton.width + 15, boundingBox.y + 4, 60, 12 );

    for(int i = 0; i < panels.size(); i++){
        panels[i]->update();

        panelTabs[i].x      = i * tabWidth + hitArea.x + borderWidth;
        panelTabs[i].y      = hitArea.y + topSpacing - tabHeight;
        panelTabs[i].width  = tabWidth;
        panelTabs[i].height = tabHeight;

    }
}

//---------------------------------------------
void ofxControlPanel::updateBoundingBox(){
    if(bShowText){
        //we need to update out hitArea because the text will have moved the gui down
        hitArea.y = boundingBox.y + topSpacing;
        boundingBox.height = hitArea.height + displayText.getTextHeight() + titleSpacing;
        boundingBox.width  = MAX( hitArea.width, displayText.getTextWidth() );
    }else{
         //we need to update out hitArea because the text will have moved the gui down
        hitArea.y = boundingBox.y;
        boundingBox.height = hitArea.height;
        boundingBox.width  = hitArea.width;
    }
}


// ############################################################## //
// ##
// ##       Drawing
// ##
// ############################################################## //

//-------------------------------
void ofxControlPanel::draw(){
    if( hidden ) return;

    ofPushStyle();
    ofEnableAlphaBlending();

        float panelH = boundingBox.height;
        if( minimize ){
            panelH = 20;
        }

        glPushMatrix();
            glTranslatef(boundingBox.x, boundingBox.y, 0);
            //draw the background
            ofFill();
            glColor4fv(bgColor.getColorF());
            ofRect(0, 0, boundingBox.width, panelH);

            //draw the outline
            ofNoFill();
            glColor4fv(outlineColor.getColorF());
            ofRect(0, 0, boundingBox.width, panelH);
            ofLine(0, 20, boundingBox.width, 20);
        glPopMatrix();

        ofRect(minimizeButton.x, minimizeButton.y, minimizeButton.width, minimizeButton.height);

        ofPushStyle();
            ofFill();

            if( saveDown )glColor4fv(fgColor.getSelectedColorF());
            else glColor4fv(fgColor.getColorF());

            ofRect(saveButton.x, saveButton.y, saveButton.width,saveButton.height);
            ofSetColor(255, 255, 255);
            ofDrawBitmapString("save", saveButton.x + 3, saveButton.y + saveButton.height -3);
        ofPopStyle();

        ofPushStyle();
            ofFill();

            if( restoreDown )glColor4fv(fgColor.getSelectedColorF());
            else glColor4fv(fgColor.getColorF());

            ofRect(restoreButton.x, restoreButton.y, restoreButton.width,restoreButton.height);
            ofSetColor(255, 255, 255);
            ofDrawBitmapString("restore", restoreButton.x + 3, restoreButton.y + restoreButton.height -3);
        ofPopStyle();


        ofPushMatrix();
            ofTranslate(2,0,0);
            glColor4fv(textColor.getColorF());
            guiBaseObject::renderText();
        ofPopMatrix();

        if( !minimize ){

            //don't let gui elements go out of their panels
            glEnable(GL_SCISSOR_TEST);
            glScissor(boundingBox.x, ofGetHeight() - ( boundingBox.y + boundingBox.height - (-2 + topSpacing) ), boundingBox.width - borderWidth , boundingBox.height);

                for(int i = 0; i < panelTabs.size(); i++){
                    if( i == selectedPanel){
                        ofPushStyle();
                            ofFill();
                            glColor4fv(fgColor.getSelectedColorF());
                            ofRect(panelTabs[i].x, panelTabs[i].y, panelTabs[i].width, panelTabs[i].height);
                            glColor4fv(outlineColor.getColorF());
                        ofPopStyle();
                    }
                    glColor4fv(outlineColor.getColorF());
                    ofNoFill();
                    ofRect(panelTabs[i].x, panelTabs[i].y, panelTabs[i].width, panelTabs[i].height);
                }

                glPushMatrix();
                    glTranslatef(hitArea.x, hitArea.y, 0);
                    for(int i = 0; i < panels.size(); i++){
                        if( i == selectedPanel )panels[i]->render();
                    }
                glPopMatrix();

            glDisable(GL_SCISSOR_TEST);
        }

    ofPopStyle();
}



