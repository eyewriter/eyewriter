/*
 *  groupCollection.h
 *  openFrameworks
 *
 *  Created by theo on 10/09/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "strokeGroup.h"

class groupCollection{

	public:
	
	//VECTOR INHERITED 
	
	//--------------------------------
	void clear(){
		groups.clear();
	}

	//--------------------------------	
	strokeGroup & operator[]( const uint i ) {
       if( groups.size() == 0 ){
			addGroup();
	   }
	   
	   int index = ofClamp(i, 0, groups.size());
	   return groups[index];
    }
	
	//--------------------------------
	uint size(){
		return groups.size();
	}
	
	//--------------------------------
	strokeGroup & back(){
		if( groups.size() == 0 ){
			addGroup();
	   }
	   
	   return groups.back();
	}

	//--------------------------------
	vector <strokeGroup> & getVector(){
		return groups;
	}
	
	
	//STROKE GROUP SPECIFIC 

	//---------------------------------------------
	void addGroup(){
		groups.push_back(strokeGroup());
	}
		
	//---------------------------------------------
	bool hasPoints(){
		for(int i = 0; i < groups.size(); i++){
			if(groups[i].hasPoints())return true;
		}
		return false;
	}

	protected:
		vector <strokeGroup> groups;

};