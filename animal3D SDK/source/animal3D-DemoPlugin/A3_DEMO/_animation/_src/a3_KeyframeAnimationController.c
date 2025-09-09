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

a3f64 clipt0(a3_ClipController* clipCtrl, const a3_Clip* clip)
{
	a3_Keyframe clipFirstKeyFrame = clipCtrl->clipPool->keyframe[clip->keyframeIndex_first];
	a3f64 clipt0 = clipCtrl->clipPool->sample[clipFirstKeyFrame.sampleIndex0].time_sec;
	return clipt0;
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
		clipCtrl->keyframeTime_sec += dt * clipCtrl->playback_sec;
		clipCtrl->clipTime_sec += dt;

		// Step 2: a. paused dt = 0
		if (dt == 0) return 0;

		if ((clipCtrl->clipTime_sec - clipCtrl->clip->duration_sec) > 0) {
			// FOR STOPPING - interpolated keyframe time and clip time == 1 
			// 
			// TRANSITION FLAGS
			return 0; // will get changed depending on transition flags
		}

		a3f64 currentClipt0 = clipt0(clipCtrl, clipCtrl->clip);

		a3f64 t0 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex0].time_sec;
		a3f64 t1 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex1].time_sec;

		// moments within the current group of clips
		a3_Sample* clipArr = clipCtrl->clipPool->sample;

		// keyframe index (0-4)
		// sample count = keyframeCount + 1

		// While current time is outside of the active keyframe
		while (clipCtrl->clipTime_sec >= t1 || clipCtrl->clipTime_sec < t0)
		{
			if (dt > 0) 
			{
				clipCtrl->keyframeIndex++;
				// Updating t0 and t1 values to ensure they get updated
				t0 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex0].time_sec;
				t1 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex1].time_sec;
				// implementing transition
				break;
			}
			else  //dt < 0  reverse
			{
				break;
			}
		}

		//Step 2: Resolve keyframe 
				//	ii. step(s) taken
				//  iii: clip exited
			// c. reverse: dt < 0
				//  i. stop
				//  ii. step(s) taken
				//  iii. clip exited
	 
		// 3. Normalized keyframe/clip: relative time / duration. Worked on primarily by Jerry (everyone was present in Joyce 101)

		// SampleIndex possibly representing time values of keyframe
			// Tristian helped and stated that it should be seen as a time step for when the keyframe starts and ends in clip time
		// u = (t - t0) / (t1 - t0)   _ t can represent the current time in terms of the clip or in terms of the clip

		// Clip Time in Seconds - t0 * the current keyframe's duration 
		clipCtrl->keyframeParam = (clipCtrl->clipTime_sec - t0) * clipCtrl->keyframe[clipCtrl->keyframeIndex].durationInv; // Multiplying the inverse instead of dividing
		clipCtrl->clipParam = clipCtrl->clipTime_sec / clipCtrl->clip->duration_sec;
 

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-1
//-----------------------------------------------------------------------------
	}

	return -1;
}


//-----------------------------------------------------------------------------
