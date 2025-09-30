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
				
				// Sierra
				// Load is currently not working, either because nothing is being saved so nothing is being loaded or the loader isn't working properly

				ret += (a3ui32)fread(&poseGroup->hposeCount, sizeof(poseGroup->hposeCount), 1, fp);

				// Determine memory requirements
				a3ui32 const nodeCount = poseGroup->hierarchy->numNodes;
				a3ui32 const hposeCount = poseGroup->hposeCount, hposeSpace = sizeof(a3_HierarchyPose) * hposeCount;
				a3ui32 const sposeCount = hposeCount * nodeCount, sposeSpace = sizeof(a3_SpatialPose) * sposeCount;
				a3ui32 const channelSpace = sizeof(a3_SpatialPoseChannel) * nodeCount;
				a3ui32 const orderSpace = sizeof(a3_SpatialPoseEulerOrder) * nodeCount;
				a3ui32 const memreq = hposeSpace + sposeSpace + channelSpace + orderSpace;
				a3index i;

				// Allocate everything (one malloc)
				poseGroup->hpose = (a3_HierarchyPose*)malloc(memreq);
				poseGroup->hpose->hpose_base = poseGroup->pose = (a3_SpatialPose*)(poseGroup->hpose + hposeCount);
				poseGroup->channel = (a3_SpatialPoseChannel*)(poseGroup->pose + sposeCount);
				poseGroup->order = (a3_SpatialPoseEulerOrder*)(poseGroup->channel + nodeCount);

				// Set pointers
				for (i = 1; i < hposeCount; ++i)
				{
					poseGroup->hpose[i].hpose_base = poseGroup->hpose[i - 1].hpose_base + nodeCount;
					poseGroup->hpose[i].hpose_index = i * nodeCount;
				}

				ret += (a3ui32)fread(poseGroup->pose, sizeof(a3_SpatialPose), poseGroup->hposeCount * poseGroup->hierarchy->numNodes, fp);
				ret += (a3ui32)fread(poseGroup->channel, sizeof(a3_SpatialPoseChannel), poseGroup->hierarchy->numNodes, fp);
				ret += (a3ui32)fread(poseGroup->channel, sizeof(a3_SpatialPoseEulerOrder), poseGroup->hierarchy->numNodes, fp);

				/*a3ui32 const nodeCount = hierarchy->numNodes;
		a3ui32 const hposeCount = poseCount, hposeSpace = sizeof(a3_HierarchyPose) * hposeCount;
		a3ui32 const sposeCount = hposeCount * nodeCount, sposeSpace = sizeof(a3_SpatialPose) * sposeCount;
		a3ui32 const channelSpace = sizeof(a3_SpatialPoseChannel) * nodeCount;
		a3ui32 const orderSpace = sizeof(a3_SpatialPoseEulerOrder) * nodeCount;
		a3ui32 const memreq = hposeSpace + sposeSpace + channelSpace + orderSpace;
		a3index i;*/

				// Allocates memory for us
				// Disregard the return value
				/*a3hierarchyPoseGroupCreate(poseGroup, poseGroup->hierarchy, poseGroup->hposeCount);
				// Calculating the number of bytes
				ret += (a3ui32)fread(poseGroup->pose, sizeof(a3_SpatialPose), poseGroup->hposeCount * poseGroup->hierarchy->numNodes, fp);
				ret += (a3ui32)fread(poseGroup->channel, sizeof(a3_SpatialPoseChannel), poseGroup->hierarchy->numNodes, fp);
				ret += (a3ui32)fread(poseGroup->channel, sizeof(a3_SpatialPoseEulerOrder), poseGroup->hierarchy->numNodes, fp);*/

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
				
				// Sierra

				// POSES
				// Save the number of hierarchy poses
				ret += (a3ui32)fwrite(&poseGroup->hposeCount, sizeof(poseGroup->hposeCount), 1, fp);
				// Save total number of bytes
				ret += (a3ui32)fwrite(poseGroup->pose, sizeof(a3_SpatialPose), poseGroup->hposeCount * poseGroup->hierarchy->numNodes, fp);

				// CHANNEL
				ret += (a3ui32)fwrite(poseGroup->channel, sizeof(a3_SpatialPoseChannel), poseGroup->hierarchy->numNodes, fp);

				//ORDER
				ret += (a3ui32)fwrite(poseGroup->order, sizeof(a3_SpatialPoseEulerOrder), poseGroup->hierarchy->numNodes, fp);

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
//****TO-DO-ANIM-PROJECT-2: DONE
//-----------------------------------------------------------------------------
		
		for (i = 0; i < state->hierarchy->numNodes; i++) {
			a3real4x4Product(state->objectSpaceBindToCurrent->hpose_base[i].transformMat.m,
				state->objectSpace->hpose_base[i].transformMat.m,
				state_bind->objectSpaceInv->hpose_base[i].transformMat.m);
		}

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
		return i;
	}
	return -1;
}

// Jerry
a3boolean parseKeyValue(FILE* file, char* key, char* value) {
	char currentLine[256];
	if (fgets(currentLine, sizeof(currentLine), file) != NULL) {
		// Checks if currentLine has any "\n" inside of it and removes it from currentLine
		currentLine[strcspn(currentLine, "\n")] = 0;

		// Checks if its a new section
		if (currentLine[0] == '[' || currentLine[0] == '#') {
			return false;
		}

		// Ensures that there are two variables that get stored on parse
		if (sscanf(currentLine, "%s %s", key, value) == 2) {
			return true;
		}
		return false;
	}
	return true;
}

// Jerry
a3boolean parseHeaderSection(FILE* animData, a3_Hierarchy* hierarchy_out, a3_HierarchyPoseGroup* poseGroup_out, int* parsedSections, a3f32* globalScale) {
	char key[100];
	char value[100];
	float unitScale = 1;
	float scaleFactor = 1;

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
			a3hierarchyCreate(hierarchy_out, atoi(value), NULL);
		}
		// Initalize pose group based on the anount of numSegments found in htr
		else if (strstr(key, "NumFrames")) {
			printf("%s\n", value);
			a3hierarchyPoseGroupCreate(poseGroup_out, hierarchy_out, atoi(value));
		}
		else if (strstr(key, "EulerRotationOrder")) {
			printf("%s\n", value); 
			if (strstr(value, "ZYX")) {
				poseGroup_out->order[0] = a3poseEulerOrder_zyx;
			}
		}
		else if (strstr(key, "CalibrationUnits")) {
			printf("%s\n", value);
			// During use case of the announcement, this is here to act as the global scale
			if (strstr(value, "mm")) {
				unitScale = 100.f / 1000.f;
			}
		}
		else if (strstr(key, "BoneLengthAxis")) {
			printf("%s\n", value);
			if (strstr(value, "Y")) {
				*poseGroup_out->channel = a3poseChannel_scale_y;
			}
		}
		else if (strstr(key, "ScaleFactor")) {
			printf("%s\n", value);
			scaleFactor = (a3f32) atof(value);
;		}
	}

	*globalScale = scaleFactor * unitScale;
	*parsedSections = 1;
	return true;
}

// Jerry
a3boolean parsePositionSection(FILE* animData, a3_Hierarchy* hierarchy_out, a3_HierarchyPoseGroup* poseGroup_out, a3f32 globalScale, int* parsedSections) {
	char currentLine[256];
	char key[a3node_nameSize]; // once we implement the hierarchy and segments it will read the 67 lines
	a3vec3 pos, rot;
	float scale;
	a3ui16 j = 0;
	a3_SpatialPose* spatialPose = 0;

	printf("\nBase Position: \n");

	while (fgets(currentLine, sizeof(currentLine), animData) != NULL) {
		if (strstr(currentLine, "# base") || currentLine[0] == '[' || currentLine[0] == '#') break;

		sscanf(currentLine, "%s %f %f %f %f %f %f %f", key, &pos.x, &pos.y, &pos.z, &rot.x, &rot.y, &rot.z, &scale);
		printf("%s\n", key);

		a3f32 posX = pos.x;
		j = a3hierarchyGetNodeIndex(hierarchy_out, key);
		spatialPose = &poseGroup_out->hpose[0].hpose_base[j];

		// Austin
		a3spatialPoseSetTranslation(spatialPose, pos.x * globalScale, pos.y * globalScale, pos.z * globalScale);
		a3spatialPoseSetRotation(spatialPose, rot.x, rot.y, rot.z);
		a3spatialPoseSetScale(spatialPose, scale, scale, scale);
	}

	*parsedSections = 3;
	return true;
}

// Jerry
a3boolean parseSegmentHierarchy(FILE* animData, a3_Hierarchy* hierarchy_out, a3_HierarchyPoseGroup* poseGroup_out, int* parsedSection) {
	char currentLine[300];
	char s[a3node_nameSize];
	char h[a3node_nameSize];

	printf("\nSegment Names & Hierarchy:\n");

	int parentIndex = 0, childIndex = 0;
	while (fgets(currentLine, sizeof(currentLine), animData) != NULL) {
		if (strstr(currentLine, "# base") || currentLine[0] == '[' || currentLine[0] == '#') break;

		sscanf(currentLine, "%s %s", s, h);
		printf("%s\n", s);
		printf("%s", h);

		parentIndex = a3hierarchyGetNodeIndex(hierarchy_out, s);

		a3hierarchySetNode(hierarchy_out, childIndex, parentIndex, h);
		childIndex++;
	}

	*parsedSection = 2;
	return true;
}



//-----------------------------------------------------------------------------

// load HTR file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadHTR(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->hierarchy && hierarchy_out && !hierarchy_out->nodes && resourceFilePath && *resourceFilePath)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: WIP
//-----------------------------------------------------------------------------
		
		// Sierra
		// Removed for sake of testing - Jerry
		// For binary loader testing
		/*{
			a3hierarchyCreate(hierarchy_out, 67, 0);
			a3hierarchyPoseGroupCreate(poseGroup_out, hierarchy_out, 2084);
			return 1;
		}*/

		//Austin
		//const a3ui32 jointCount = 32;

		//// indices of joints, their parents and branching joints
		//a3ui32 jointIndex = 0;
		//a3i32 jointParentIndex = -1;
		//a3i32 rootJointIndex;
		//a3i32 p = 0;
		//a3i32 j = 0;
		//// initialize hierarchy
		//a3hierarchyCreate(hierarchy_out, jointCount, 0);

		//// set up joint relationships
		//jointParentIndex = rootJointIndex = a3hierarchySetNode(hierarchy_out, jointIndex++, jointParentIndex, "Hips");
		//jointParentIndex = a3hierarchySetNode(hierarchy_out, jointIndex++, jointParentIndex, "Spine");
		////jointParentIndex = a3hierarchySetNode(hierarchy_out, jointIndex++, jointParentIndex, "skel:spine_mid");

		//// allocate poses
		//a3hierarchyPoseGroupCreate(poseGroup_out, hierarchy_out, 2);

		//// define "bind pose" or "base pose" or the initial transformation
		////	description for each joint (not a literal transform)
		//p = 0;
		//j = a3hierarchyGetNodeIndex(hierarchy_out, "Hips");
		//poseGroup_out->pose = poseGroup_out->hpose[p].hpose_base + j;
		//a3spatialPoseSetRotation(poseGroup_out->pose, 0.0f, -90.0f, -5.0f);
		//a3spatialPoseSetTranslation(poseGroup_out->pose, 0.0f, -0.1f, +0.1f);
		//a3spatialPoseSetTranslation(poseGroup_out->pose, 0.0f, 0.0f, +3.6f);
		//poseGroup_out->channel[j] = a3poseChannel_rotate_xyz | a3poseChannel_scale_xyz | a3poseChannel_translate_xyz;

		//j = a3hierarchyGetNodeIndex(hierarchy_out, "Spine");
		//poseGroup_out->pose = poseGroup_out->hpose[p].hpose_base + j;
		//a3spatialPoseSetRotation(poseGroup_out->pose, 0.0f, -90.0f, -5.0f);
		//a3spatialPoseSetTranslation(poseGroup_out->pose, 0.0f, +5.1f, +0.1f);
		//poseGroup_out->channel[j] = a3poseChannel_rotate_xyz;


		// Jerry
		FILE* animData = fopen(resourceFilePath, "r");
		char currentLine[100];
		a3f32 globalScale = 1;
		int parsedSections = 0;

		if (animData) {
			// reads each line until the end of
			while (fgets(currentLine, sizeof(currentLine), animData) != NULL) {
				if (parsedSections == 0 && strstr(currentLine, "[Header]")) {
					// header cannot be parsed
					if (!parseHeaderSection(animData, hierarchy_out, poseGroup_out, &parsedSections, &globalScale)) {
						return -1;
					}
				}

				else if (parsedSections == 1) {
					if (!parseSegmentHierarchy(animData, hierarchy_out, poseGroup_out, &parsedSections)) {
						return -1;
					}
				}

				else if (parsedSections == 2 && strstr(currentLine, "[BasePosition]")) {
					// base pos cannot be parsed
					if (!parsePositionSection(animData, hierarchy_out, poseGroup_out, globalScale, &parsedSections)) {
						return -1;
					}
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
