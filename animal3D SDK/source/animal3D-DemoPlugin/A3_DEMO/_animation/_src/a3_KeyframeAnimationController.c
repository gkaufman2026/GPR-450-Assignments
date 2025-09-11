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

// Added for absolute valueing the playback speed
#include <math.h> 

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
		// Austin
		dt *= clipCtrl->playback_sec;
		clipCtrl->keyframeTime_sec += dt;
		clipCtrl->clipTime_sec += dt;

		// Step 2: a. paused dt = 0
		if (dt == 0) return 0;
		
		// Creating variables for t0 and t1 to simplify having to type out all this every time
		a3f64 t0 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex0].time_sec;
		a3f64 t1 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex1].time_sec;

		// Checking if clip time is greater than clip duration (overstep)
		if (clipCtrl->clipTime_sec > clipCtrl->clip->duration_sec) {
			// TRANSITION FLAGS

			// Jerry and Austin
			switch (clipCtrl->clip->transitionForward->flag) {
			case a3clip_stopFlag:
				// stop at end
				// Setting the direction to 0 and setting the clip and keytime interpolation to 1 
				clipCtrl->clipPool->clip->keyframeDirection = 0;
				clipCtrl->keyframeParam = 1;
				clipCtrl->clipParam = 1;
				clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_final;
				return 0;
			case a3clip_playFlag:
				// @TODO - Loop or repeat at end if playing forward

				clipCtrl->keyframeIndex = 0;
				//adds remaining time onto the new clip
				clipCtrl->clipTime_sec = 0 + (clipCtrl->clipTime_sec - clipCtrl->clip->duration_sec);
				t0 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex0].time_sec;
				t1 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex1].time_sec;
				break;
			case a3clip_reverseFlag:
				// flip playback direction
				// @TODO do we want this to be a absolute value?
				clipCtrl->playback_sec *= -1;
				return 0;
			}

			switch (clipCtrl->clip->transitionReverse->flag) {
			case a3clip_stopFlag:
				// stop at beginning
				clipCtrl->clipPool->clip->keyframeDirection = 0;
				clipCtrl->keyframeParam = 1;
				clipCtrl->clipParam = 1;
				clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_first;
				return 0;
			case a3clip_playFlag:
				// @TODO - Loop or repeat at beginning if playing reverse
				//NEW
				clipCtrl->keyframeIndex = clipCtrl->clip->keyframeCount;
				//adds remaining time onto the new clip
				//NEW- 0 is replaced by - addditional time backwards
				clipCtrl->clipTime_sec = 0 + (clipCtrl->clipTime_sec - clipCtrl->clip->duration_sec);
				t0 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex0].time_sec;
				t1 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex1].time_sec;

				break;
			case a3clip_reverseFlag:
				clipCtrl->playback_sec *= -1;
				return 0;
			}
		}

		// hastebin with code that we're re-implementing https://hastebin.com/share/mifejureke.rust

		// moments within the current group of clips
		a3_Sample* clipArr = clipCtrl->clipPool->sample;

		// keyframe index (0-4)
		// sample count = keyframeCount + 1

		// While current time is outside of the active keyframe
		// Jerry
		while (clipCtrl->clipTime_sec >= t1 || clipCtrl->clipTime_sec < t0)
		{
			// if forwards playing
			// Jerry and Austin
			if (dt > 0) 
			{
				clipCtrl->keyframeIndex++;
				// Updating t0 and t1 values to ensure they get updated
				t0 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex0].time_sec;
				t1 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex1].time_sec;
				break;
			}
			// If backwards playing
			// Austin
			else
			{
				// Austin - Jerry is disappointed in him.
				if (clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex0].time_sec == 0) {
					clipCtrl->playback_sec = 1;
					break;
				}

				// Jerry and Austin
				clipCtrl->keyframeIndex--;
				t0 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex0].time_sec;
				break;
			}
		}
	 
		// 3. Normalized keyframe/clip: relative time / duration. Worked on primarily by Jerry (everyone was present in Joyce 101)

		// SampleIndex possibly representing time values of keyframe
			// Tristian helped and stated that it should be seen as a time step for when the keyframe starts and ends in clip time
		// u = (t - t0) / (t1 - t0)   _ t can represent the current time in terms of the clip or in terms of the clip

		// Clip Time in Seconds - t0 * the current keyframe's duration 
		clipCtrl->keyframeParam = (clipCtrl->clipTime_sec - t0) * clipCtrl->keyframe[clipCtrl->keyframeIndex].durationInv; // Multiplying the inverse instead of dividing
		clipCtrl->clipParam = clipCtrl->clipTime_sec * clipCtrl->clip->durationInv;
 

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-1
//-----------------------------------------------------------------------------
	}

	return -1;
}


//-----------------------------------------------------------------------------
