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
	
	a3_HierarchyState.c
	Implementation of transform hierarchy state.
*/

#include "../a3_HierarchyState.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//-----------------------------------------------------------------------------

// initialize pose set given an initialized hierarchy and key pose count
a3i32 a3hierarchyPoseGroupCreate(a3_HierarchyPoseGroup *poseGroup_out, const a3_Hierarchy *hierarchy, const a3ui32 poseCount)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (poseGroup_out && hierarchy && !poseGroup_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements
		a3ui32 const nodeCount = hierarchy->numNodes;
		a3ui32 const hposeCount = poseCount, hposeSpace = sizeof(a3_HierarchyPose) * hposeCount;
		a3ui32 const sposeCount = hposeCount * nodeCount, sposeSpace = sizeof(a3_SpatialPose) * sposeCount;
		a3ui32 const channelSpace = sizeof(a3_SpatialPoseChannel) * nodeCount;
		a3ui32 const orderSpace = sizeof(a3_SpatialPoseEulerOrder) * nodeCount;
		a3ui32 const memreq = hposeSpace + sposeSpace + channelSpace + orderSpace;
		a3index i;

		// allocate everything (one malloc)
		poseGroup_out->hpose = (a3_HierarchyPose*)malloc(memreq);
		poseGroup_out->hpose->hpose_base = poseGroup_out->pose = (a3_SpatialPose*)(poseGroup_out->hpose + hposeCount);
		poseGroup_out->channel = (a3_SpatialPoseChannel*)(poseGroup_out->pose + sposeCount);
		poseGroup_out->order = (a3_SpatialPoseEulerOrder*)(poseGroup_out->channel + nodeCount);

		// set pointers
		for (i = 1; i < hposeCount; ++i)
		{
			poseGroup_out->hpose[i].hpose_base = poseGroup_out->hpose[i - 1].hpose_base + nodeCount;
			poseGroup_out->hpose[i].hpose_index = i * nodeCount;
		}

		// reset all data
		a3hierarchyPoseReset(poseGroup_out->hpose, sposeCount);
		memset(poseGroup_out->channel, a3poseChannel_none, channelSpace);
		memset(poseGroup_out->order, a3poseEulerOrder_xyz, orderSpace);
		poseGroup_out->hierarchy = hierarchy;
		poseGroup_out->hposeCount = hposeCount;
		poseGroup_out->poseCount = sposeCount;

		// done
		return 1;
	}
	return -1;
}

// release pose set
a3i32 a3hierarchyPoseGroupRelease(a3_HierarchyPoseGroup *poseGroup)
{
	// validate param exists and is initialized
	if (poseGroup && poseGroup->hierarchy)
	{
		// release everything (one free)
		free(poseGroup->hpose);

		// reset pointers
		poseGroup->hierarchy = 0;
		poseGroup->hpose = 0;
		poseGroup->pose = 0;
		poseGroup->channel = 0;
		poseGroup->order = 0;

		// done
		return 1;
	}
	return -1;
}

// load binary
a3i32 a3hierarchyPoseGroupLoadBinary(a3_HierarchyPoseGroup* poseGroup, a3_FileStream const* fileStream)
{
	FILE* fp;
	a3ui32 ret = 0;
	a3ui32 dataSize = 0;
	if (poseGroup && fileStream)
	{
		if (poseGroup->hierarchy && !poseGroup->pose)
		{
			fp = fileStream->stream;
			if (fp)
			{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------
				


//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
			}
			return ret;
		}
	}
	return -1;
}

// save binary
a3i32 a3hierarchyPoseGroupSaveBinary(a3_HierarchyPoseGroup const* poseGroup, a3_FileStream const* fileStream)
{
	FILE* fp;
	a3ui32 ret = 0;
	if (poseGroup && fileStream)
	{
		if (poseGroup->hierarchy && poseGroup->pose)
		{
			fp = fileStream->stream;
			if (fp)
			{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------
				


//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
			}
			return ret;
		}
	}
	return -1;
}


//-----------------------------------------------------------------------------

// initialize hierarchy state given an initialized hierarchy
a3i32 a3hierarchyStateCreate(a3_HierarchyState *state_out, const a3_Hierarchy *hierarchy)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (state_out && hierarchy && !state_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements
		a3ui32 const nodeCount = hierarchy->numNodes;
		a3ui32 const hposeCount = sizeof(state_out->hpose) / sizeof(a3_HierarchyPose);
		a3ui32 const sposeCount = hposeCount * nodeCount;
		a3ui32 const memreq = sizeof(a3_SpatialPose) * sposeCount;
		a3index i;

		// allocate everything (one malloc)
		state_out->hpose->hpose_base = (a3_SpatialPose*)malloc(memreq);

		// set pointers
		state_out->hierarchy = hierarchy;
		for (i = 1; i < hposeCount; ++i)
		{
			state_out->hpose[i].hpose_base = state_out->hpose[i - 1].hpose_base + nodeCount;
			state_out->hpose[i].hpose_index = i * nodeCount;
		}

		// reset all data
		a3hierarchyPoseReset(state_out->hpose, sposeCount);

		// done
		return 1;
	}
	return -1;
}

// release hierarchy state
a3i32 a3hierarchyStateRelease(a3_HierarchyState *state)
{
	// validate param exists and is initialized
	if (state && state->hierarchy)
	{
		a3ui32 const hposeCount = sizeof(state->hpose) / sizeof(a3_HierarchyPose);
		a3ui32 i;

		// release everything (one free)
		free(state->hpose->hpose_base);

		// reset pointers
		state->hierarchy = 0;
		for (i = 0; i < hposeCount; ++i)
		{
			state->hpose[i].hpose_base = 0;
			state->hpose[i].hpose_index = 0;
		}

		// done
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// update inverse local-space matrices
a3i32 a3hierarchyStateUpdateLocalInverse(const a3_HierarchyState* state)
{
	if (state && state->hierarchy)
	{
		a3index i = 0;
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: DONE 
//-----------------------------------------------------------------------------
		
		for (i = 0; i < state->hierarchy->numNodes; i++) {
			a3real4x4TransformInverse(state->localSpaceInv->hpose_base[i].transformMat.m,
				state->objectSpace->hpose_base[i].transformMat.m);
		}

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
		return i;
	}
	return -1;
}

// update inverse object-space matrices
a3i32 a3hierarchyStateUpdateObjectInverse(const a3_HierarchyState* state)
{
	if (state && state->hierarchy)
	{
		a3index i = 0;
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: DONE
//-----------------------------------------------------------------------------
		
		for (i = 0; i < state->hierarchy->numNodes; i++) {
			a3real4x4TransformInverse(state->objectSpaceInv->hpose_base[i].transformMat.m,
				state->objectSpace->hpose_base[i].transformMat.m);
		}

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
		return i;
	}
	return -1;
}

// update inverse object-space bind-to-current matrices
a3i32 a3hierarchyStateUpdateObjectBindToCurrent(const a3_HierarchyState* state, const a3_HierarchyState* state_bind)
{
	if (state && state->hierarchy && state_bind && state_bind->hierarchy)
	{
		a3index i = 0;
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		
		for (i = 0; i < state->hierarchy->numNodes; i++) {
			a3real4x4TransformInverse(state_bind->objectSpaceBindToCurrent->hpose_base[i].transformMat.m,
				state->objectSpace->hpose_base[i].transformMat.m);
		}

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
		return i;
	}
	return -1;
}

int parseKeyValue(FILE* file, char* key, char* value) {
	char currentLine[256];
	if (fgets(currentLine, sizeof(currentLine), file) != NULL) {
		// Checks if currentLine has any "\n" inside of it and removes it from currentLine
		currentLine[strcspn(currentLine, "\n")] = 0;

		// Checks if its a new section
		if (currentLine[0] == '[') {
			return 0;
		}

		if (sscanf(currentLine, "%s %s", key, value) == 2) {
			return 1;
		}
		return 0;
	}
	return 0;
}

a3boolean parseHeaderSection(FILE * animData, a3_Hierarchy * hierarchy_out, a3_HierarchyPoseGroup * poseGroup_out) {
	char key[100];
	char value[100];

	while (parseKeyValue(animData, key, value)) {
		// Read the keys to the associated map
		if (strstr(key, "FileType")) {
			printf("%s\n", value);
			if (!strstr(value, "HTR")) return false;
		} 
		else if (strstr(key, "DataType")) {
			printf("%s\n", value);
			if (!strstr(value, "HTRS")) return false;
		} 
		// Convert string to int
		else if (strstr(key, "FileVersion")) {
			printf("%s\n", value);
			if (atoi(value) != 1) return false;
		}
		// Create hierarchy based on the anount of numSegments found in htr
		else if (strstr(key, "NumSegments")) {
			printf("%s\n", value);
			//a3hierarchyCreate(hierarchy_out, atoi(value), NULL);
		}
		// Initalize pose group based on the anount of numSegments found in htr
		else if (strstr(key, "NumFrames")) {
			printf("%s\n", value);
			printf("%s\n", value);
			//a3hierarchyPoseGroupCreate(poseGroup_out, hierarchy_out, atoi(value));
		}
		// Need to implement DataFrameRate
		else if (strstr(key, "DataFrameRate")) {
			printf("%s\n", value);
			// Not the right function
			//a3hierarchyPoseGroupCreate(poseGroup_out, hierarchy_out, atoi(value));
		} 
		else if (strstr(key, "EulerRotationOrder")) {
			if (strcmp(value, "a3poseEulerOrder_xyz")) {
				poseGroup_out->order = (a3_SpatialPoseEulerOrder*) a3poseEulerOrder_xyz;
				break;
			} else if (strcmp(value, "a3poseEulerOrder_yzx")) {
				poseGroup_out->order = (a3_SpatialPoseEulerOrder*)a3poseEulerOrder_yzx;
				break;
			} else if (strcmp(value, "a3poseEulerOrder_zxy")) {
				poseGroup_out->order = (a3_SpatialPoseEulerOrder*)a3poseEulerOrder_zxy;
				break;
			} else if (strcmp(value, "a3poseEulerOrder_yxz")) {
				poseGroup_out->order = (a3_SpatialPoseEulerOrder*)a3poseEulerOrder_yxz;
				break;
			} else if (strcmp(value, "a3poseEulerOrder_xzy")) {
				poseGroup_out->order = (a3_SpatialPoseEulerOrder*)a3poseEulerOrder_xzy;
				break;
			} else if (strcmp(value, "a3poseEulerOrder_zyx")) {
				poseGroup_out->order = (a3_SpatialPoseEulerOrder*)a3poseEulerOrder_zyx;
				break;
			}

		}
		else if (strstr(key, "CalibrationUnits")) {
			printf("%s\n", value);
			//a3hierarchyCreate(hierarchy_out, atoi(value), NULL);
		}
		else if (strstr(key, "RotationUnits")) {
			printf("%s\n", value);
			//a3hierarchyCreate(hierarchy_out, atoi(value), NULL);
		}
		else if (strstr(key, "BoneLengthAxis")) {
			printf("%s\n", value);
			//a3hierarchyCreate(hierarchy_out, atoi(value), NULL);
		}
		else if (strstr(key, "ScaleFactor")) {
			printf("%s\n", value);
			//a3hierarchyCreate(hierarchy_out, atoi(value), NULL);
		}
	}

	return true;
}

a3boolean parsePositionSection(FILE* animData, a3_Hierarchy* hierarchy_out, a3_HierarchyPoseGroup* poseGroup_out) {
	char currentLine[256];
	char key[a3node_nameSize]; // once we implement the hierarchy and segments it will read the 67 lines
	a3vec3 pos, rot;
	float scale;

	if (fgets(currentLine, sizeof(currentLine), animData) != NULL) {
		sscanf(currentLine, "%s %f %f %f %f %f %f %f", key, &pos.x, &pos.y, &pos.z, &rot.x, &rot.y, &rot.z, &scale);
		printf("%s\n", key);
		a3f32 posX = pos.x;
		printf("%f, %f, %f", pos.x, pos.y, pos.z);
		printf("\n%f, %f, %f", rot.x, rot.y, rot.z);
		printf("\n%f", scale);
		int node = a3hierarchyGetNodeIndex(hierarchy_out, key);

		a3spatialPoseSetTranslation(&poseGroup_out->hpose[0].hpose_base[node], pos.x, pos.y, pos.z);
		a3spatialPoseSetRotation(&poseGroup_out->hpose[0].hpose_base[node], rot.x, rot.y, rot.z);
		a3spatialPoseSetScale(&poseGroup_out->hpose[0].hpose_base[node], scale, scale, scale);
		poseGroup_out->hpose[0].hpose_index = node;	
	}

	return true;
}

a3boolean parseSegmentHierarchy(FILE* animData, a3_Hierarchy* hierarchy_out, a3_HierarchyPoseGroup* poseGroup_out) {
	char currentLine[256];
	char s[a3node_nameSize];
	char h[a3node_nameSize];

	if (fgets(currentLine, sizeof(currentLine), animData) != NULL) {
		sscanf(currentLine, "%s %s", s, h);
		printf("%s\n", s);
		printf("%s", h);
	}

	return true;
}



//-----------------------------------------------------------------------------

// load HTR file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadHTR(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->hierarchy && hierarchy_out && !hierarchy_out->nodes && resourceFilePath && *resourceFilePath)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		
		////Austin
		////makes filestream and sets to reading
		//a3_FileStream fileStream[1] = { 0 };
		//a3fileStreamOpenRead(fileStream, resourceFilePath);

		//FILE *fp;
		//fp = fileStream->stream;

		////keysmash to store data
		//a3byte *d = "awrfwef";

		////TESTING
		//if (fileStream) {
		//	if (fp) {
		//		//reads first line (up to 100 chars) and stores in d (the keysmash var rn).
		//		//we just needa make this but automatic instead of manual setup (see a3sceneanimationload)
		//		//use the debug menu to see the value for d Jerry :p
		//		(a3byte)fgets(d, 100, fp);
		//		(a3byte)fgets(d, 100, fp);
		//		(a3byte)fgets(d, 100, fp);
		//		(a3byte)fgets(d, 100, fp);
		//		(a3byte)fgets(d, 100, fp);

		//	}
		//}

		// Jerry
		FILE* animData = fopen(resourceFilePath, "r");
		char currentLine[100];

		if (animData) {
			// reads each line until the end of
			while (fgets(currentLine, sizeof(currentLine), animData) != NULL) {
				if (strstr(currentLine, "[Header]")) {
					// header cannot be parsed
					if (!parseHeaderSection(animData, hierarchy_out, poseGroup_out)) {
						return -1;
					}
				}

				if (strstr(currentLine, "[SegmentNames&Hierarchy]")) {
					if (!parseSegmentHierarchy(animData, hierarchy_out, poseGroup_out)) {
						return -1;
					}
				}

				if (strstr(currentLine, "[BasePosition]")) {
					// base pos cannot be parsed
					//if (!parsePositionSection(animData, hierarchy_out, poseGroup_out)) {
						//return -1;
					//}
				}
			}
			// Has completed file
		} else {
			printf("Cannot find file");
			return -1;
		}

		fclose(animData);
//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
	return 1;
}

// load BVH file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadBVH(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->hierarchy && hierarchy_out && !hierarchy_out->nodes && resourceFilePath && *resourceFilePath)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------

		

//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
	}
	return -1;
}

// save HTR file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupSaveHTR(const a3_HierarchyPoseGroup* poseGroup_in, const a3_Hierarchy* hierarchy_in, const a3byte* resourceFilePath)
{
	if (poseGroup_in && poseGroup_in->hierarchy && hierarchy_in && hierarchy_in->nodes && resourceFilePath && *resourceFilePath)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
	}
	return -1;
}

// save BVH file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupSaveBVH(const a3_HierarchyPoseGroup* poseGroup_in, const a3_Hierarchy* hierarchy_in, const a3byte* resourceFilePath)
{
	if (poseGroup_in && poseGroup_in->hierarchy && hierarchy_in && hierarchy_in->nodes && resourceFilePath && *resourceFilePath)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------
