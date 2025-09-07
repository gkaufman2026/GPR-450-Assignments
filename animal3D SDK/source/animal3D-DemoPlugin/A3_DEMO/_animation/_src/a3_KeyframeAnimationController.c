/*
	Copyright 2011-2025 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	a3_KeyframeAnimationController.c
	Implementation of keyframe animation controller.
*/

#include "../a3_KeyframeAnimationController.h"

#include <string.h>

// Added for debugging printf - Jerry
#include <stdio.h>

// macros to help with names
#define A3_CLIPCTRL_DEFAULTNAME		("unnamed clip ctrl")
#define A3_CLIPCTRL_SEARCHNAME		((ctrlName && *ctrlName) ? ctrlName : A3_CLIPCTRL_DEFAULTNAME)


//-----------------------------------------------------------------------------

// initialize clip controller
a3i32 a3clipControllerInit(a3_ClipController* clipCtrl_out, const a3byte ctrlName[a3keyframeAnimation_nameLenMax], const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool, const a3i32 playback_step, const a3f64 playback_stepPerSec)
{
	a3i32 const ret = a3clipControllerSetClip(clipCtrl_out, clipPool, clipIndex_pool, playback_step, playback_stepPerSec);
	if (ret >= 0)
	{
		strncpy(clipCtrl_out->name, A3_CLIPCTRL_SEARCHNAME, a3keyframeAnimation_nameLenMax);
		return ret;
	}
	return -1;
}

// update clip controller
a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, a3f64 dt)
{
	if (clipCtrl && clipCtrl->clipPool)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-1: IMPLEMENT ME
//-----------------------------------------------------------------------------
		//Step 1: Time step - add dt
		
		// Step 2: a. paused dt = 0
		if (dt == 0) return 0;
		
		clipCtrl->keyframeTime_sec += dt;
		clipCtrl->clipTime_sec += dt;

		if (clipCtrl->keyframeTime_sec >= 1) {
			clipCtrl->keyframeIndex++;
			clipCtrl->keyframeTime_sec = 0;
		}


		//Step 2: Resolve keyframe - b. forward: dt > 0
				//	i. stop
				//	ii. step(s) taken
				//  iii: clip exited
			// c. reverse: dt < 0
				//  i. stop
				//  ii. step(s) taken
				//  iii. clip exited
	 
		// 3. Normalized keyframe/clip: relative time / duration. Worked on by Jerry (everyone was present in Joyce 101)
		double clipTimeSec = clipCtrl->clipTime_sec;
		double keyframeSec = clipCtrl->keyframeTime_sec;
		double clipDurSec = clipCtrl->clip->duration_sec;

		// t can equal the time within keyframe or clip

		// moments within the current group of clips
		a3_Sample* clipArr = clipCtrl->clipPool->sample;

		// Multiplying the inverse instead of dividing
		// SampleIndex possibly representing time values of keyframe 
			// Tristian helped and stated that it should be seen as a time step for when the keyfrae starts and ends in clip time
		clipCtrl->keyframeParam = (keyframeSec - clipArr[clipCtrl->keyframe->sampleIndex0].time_sec) * clipCtrl->keyframe->durationInv;
		clipCtrl->clipParam = clipTimeSec / clipDurSec;
 

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-1
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------
