#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

#define LOCK_WIDTH 10
#define LOCK_HEIGHT 10
#define LOCK_BORDER 3

class guiTypePanel : public guiBaseObject{

    public:

        //------------------------------------------------
        guiTypePanel(){
            currentXPos     = 20;
            currentYPos     = 20;
            spacingAmntX    = 16;
            spacingAmntY    = 16;

            columns.clear();
            columns.push_back(ofRectangle(20, 20, 30, 20));
            col = 0;
        }

        //------------------------------------------------
        void setup(string panelName, float defaultX = 20, float defaultY = 20){
            name = panelName;

            columns[0] = ofRectangle(defaultX, defaultY, 50, 20);

            //we don't want our panel flashing when we click :)
            bgColor.selected = bgColor.color;
            outlineColor.selected = outlineColor.color;
        }

        //-----------------------------------------------
        void addColumn(float minWidth){
            float colX = columns.back().x + columns.back().width + spacingAmntX;
            columns.push_back(ofRectangle(colX, 20, minWidth, 20));
        }

        //-----------------------------------------------
        bool selectColumn(int which){
            col = ofClamp(which, 0, columns.size()-1);
            return true;
        }

        //-----------------------------------------------
        void setElementSpacing(float spacingX, float spacingY){
            spacingAmntX = spacingX;
            spacingAmntY = spacingY;
        }

        //-----------------------------------------------.
        virtual bool checkHit(float x, float y, bool isRelative){
            if(readOnly)return false;
            if( x >= hitArea.x && x <= hitArea.x + hitArea.width && y >= hitArea.y && y <= hitArea.y + hitArea.height){
                state = SG_STATE_SELECTED;
                
				float xx = x - boundingBox.x;
                float yy = y - boundingBox.y;
				
				if( xx  > lockRect.x && xx < lockRect.x + lockRect.width && yy > lockRect.y && yy < lockRect.y + lockRect.height ){
                    locked = !locked;
                } 
					
                setSelected();
                updateGui(x, y, true, isRelative);
				
                if( !locked ){

                    float offsetX = x - hitArea.x;
                    float offsetY = y - hitArea.y;

                    for(int i = 0; i < children.size(); i++){
                        children[i]->checkHit(offsetX, offsetY, isRelative);
                    }
                }
                return true;
            }
            return false;
        }

        //-----------------------------------------------.
        void updateGui(float x, float y, bool firstHit, bool isRelative){
            if( state == SG_STATE_SELECTED){

                float offsetX = 0;
                float offsetY = 0;

                if( isRelative ){
                    offsetX = x;
                    offsetY = y;
                }else{
                    offsetX = x - hitArea.x;
                    offsetY = y - hitArea.y;
                }

                if( !locked ){

                    for(int i = 0; i < children.size(); i++){
                        children[i]->updateGui(offsetX, offsetY, firstHit, isRelative);
                    }

                }
            }
        }

        //we should actually be checking our child heights
        //every frame to see if the panel needs to adjust layout
        //for now we only check heights when elements are first added
        //----------------------------------------------
        virtual void update(){
            updateText();

            lockRect.x          = boundingBox.width - (LOCK_WIDTH + spacingAmntX + LOCK_BORDER);
            lockRect.y          = spacingAmntY - LOCK_BORDER;
            lockRect.width      = LOCK_WIDTH + LOCK_BORDER * 2;
            lockRect.height     = LOCK_HEIGHT + LOCK_BORDER * 2;

            for(int i = 0; i < children.size(); i++){
                children[i]->update();
            }

            for(int i = 0; i < whichColumn.size(); i++){
                if( children[i]->boundingBox.x != columns[whichColumn[i]].x ){
                    float amntToShiftX = columns[whichColumn[i]].x - children[i]->boundingBox.x;

                    children[i]->hitArea.x += amntToShiftX;
                    children[i]->boundingBox.x += amntToShiftX;
                }
            }
        }

        //-----------------------------------------------
        void addElement( guiBaseObject * element ){
            element->updateText();
            element->setPosition(columns[col].x, columns[col].y);
            whichColumn.push_back(col);

            //add the element to the panel list
            children.push_back( element );

            //update the current position for the next element
            columns[col].y += element->getHeight() + spacingAmntY;

            float checkWidth = element->getWidth();
            if(checkWidth >= columns[col].width ){
                float amnt = checkWidth - columns[col].width;
                columns[col].width += amnt;

                for(int i = col+1; i < columns.size(); i++){
                    columns[i].x += amnt;
                }
            }

            //see if we need to resize!
            //checkResize(element);

        }

        //-----------------------------------------------.
        void drawLocked(){
            ofPushMatrix();
                ofFill();
                ofTranslate(lockRect.x, lockRect.y, 0);

                ofSetColor(200, 0, 0);
                ofRect(0, 0, lockRect.width, lockRect.height);

                ofTranslate(LOCK_BORDER, LOCK_BORDER, 0);

                ofSetColor(255, 255, 255);
                ofEllipse(LOCK_WIDTH/2, LOCK_HEIGHT/2, LOCK_WIDTH * 0.8, LOCK_HEIGHT * 0.9);

                ofSetColor(200, 0, 0);
                ofEllipse(LOCK_WIDTH/2, LOCK_HEIGHT/2, LOCK_WIDTH * 0.8 * 0.6, LOCK_HEIGHT * 0.9 * 0.6);

                ofSetColor(255, 255, 255);
                ofRect(0, LOCK_HEIGHT/2, LOCK_WIDTH, LOCK_HEIGHT/2);
            ofPopMatrix();
        }

        //-----------------------------------------------.
        void drawUnlocked(){
            ofPushMatrix();
                ofFill();
                ofTranslate(lockRect.x, lockRect.y, 0);

                ofSetColor(0, 0, 0);
                ofRect(0, 0, lockRect.width, lockRect.height);

                ofTranslate(LOCK_BORDER, LOCK_BORDER, 0);
                ofSetColor(255, 255, 255);
                ofEllipse(LOCK_WIDTH/2, LOCK_HEIGHT * 0.4, LOCK_WIDTH * 0.8, LOCK_HEIGHT * 0.9);

                ofSetColor(0, 0, 0);
                ofEllipse(LOCK_WIDTH/2, LOCK_HEIGHT * 0.44, LOCK_WIDTH * 0.8 * 0.6, LOCK_HEIGHT * 0.9 * 0.6);

                ofSetColor(255, 255, 255);
                ofRect(0, LOCK_HEIGHT/2, LOCK_WIDTH, LOCK_HEIGHT/2);

                ofSetColor(0, 0, 0);
                ofRect(0, LOCK_HEIGHT * 0.5 - LOCK_HEIGHT * 0.25 , LOCK_WIDTH * 0.35, LOCK_HEIGHT * 0.25);

            ofPopMatrix();
        }

        //-----------------------------------------------.
        void render(){
            ofPushStyle();

                glPushMatrix();
                glTranslatef(boundingBox.x, boundingBox.y, 0);
                    //draw the background
                    ofFill();
                    glColor4fv(bgColor.getColorF());
                    ofRect(0, 0, boundingBox.width, boundingBox.height);

                    //draw the outline
                    ofNoFill();
                    glColor4fv(outlineColor.getColorF());
                    ofRect(0, 0, boundingBox.width, boundingBox.height);

                    if( locked ){
                        drawLocked();
                    }else{
                        drawUnlocked();
                    }

                glPopMatrix();
                renderText();

                glPushMatrix();
                    glTranslatef(hitArea.x, hitArea.y, 0);
                        for(int i = 0; i < children.size(); i++){
                            children[i]->render();
                        }
                glPopMatrix();


            ofPopStyle();
        }

        ofRectangle lockRect;

        float currentXPos;
        float currentYPos;
        float spacingAmntX;
        float spacingAmntY;

        vector <ofRectangle> columns;
        vector <int> whichColumn;
        int col;

};
