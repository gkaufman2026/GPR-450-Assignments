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
	
	a3_Kinematics.c
	Implementation of kinematics solvers.
*/

#include "../a3_Kinematics.h"
#include <stdio.h>


//-----------------------------------------------------------------------------

// single FK helpers
static inline void a3kinematicsSolveForwardSingle(const a3_HierarchyState* hierarchyState, const a3ui32 index, const a3ui32 parentIndex)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// T[this_object] = T[parent_object] * T[this_local]
	a3real4x4Product(
		hierarchyState->objectSpace->hpose_base[index].transformMat.m,		// Result: this node object-space.
		hierarchyState->objectSpace->hpose_base[parentIndex].transformMat.m,// Left-hand: parent node object-space.
		hierarchyState->localSpace->hpose_base[index].transformMat.m		// Right-hand: this node local space.
	);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
}
static inline void a3kinematicsSolveForwardRoot(const a3_HierarchyState* hierarchyState, const a3ui32 index)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// T[root_object] = T[root_local]
	hierarchyState->objectSpace->hpose_base[index] = hierarchyState->localSpace->hpose_base[index];

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
}

// partial FK solver
a3i32 a3kinematicsSolveForwardPartial(const a3_HierarchyState* hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount)
{
	if (hierarchyState && hierarchyState->hierarchy &&
		firstIndex < hierarchyState->hierarchy->numNodes && nodeCount)
	{
		// implement forward kinematics algorithm: 
		//	- for all nodes starting at first index
		//		- if node is not root (has parent node)
		//			- object matrix = parent object matrix * local matrix
		//		- else
		//			- copy local matrix to object matrix
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

		const a3_HierarchyNode* itr = hierarchyState->hierarchy->nodes + firstIndex;
		const a3_HierarchyNode* const end = itr + nodeCount;
		for (; itr < end; ++itr)
		{
			if (itr->parentIndex >= 0)
				a3kinematicsSolveForwardSingle(hierarchyState, itr->index, itr->parentIndex);
			else
				a3kinematicsSolveForwardRoot(hierarchyState, itr->index);
		}
		return (a3i32)(end - itr);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------

// single IK helpers
static inline void a3kinematicsSolveInverseSingle(const a3_HierarchyState* hierarchyState, const a3ui32 index, const a3ui32 parentIndex)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: DONE IN CLASS
//-----------------------------------------------------------------------------

	// T[this_local] = T[parent_object]^-1 * T[this_object]
	a3real4x4Product(
		hierarchyState->localSpace->hpose_base[index].transformMat.m,			// Result: this node local-space.
		hierarchyState->objectSpaceInv->hpose_base[parentIndex].transformMat.m, // Left-hand: parent node object-space inverse.
		hierarchyState->objectSpace->hpose_base[index].transformMat.m			// Right-hand: this node object space.
	);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}
static inline void a3kinematicsSolveInverseRoot(const a3_HierarchyState* hierarchyState, const a3ui32 index)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: DONE IN CLASS
//-----------------------------------------------------------------------------

	// T[root_local] = T[root_object]
	hierarchyState->localSpace->hpose_base[index] = hierarchyState->objectSpace->hpose_base[index];

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}

// partial IK solver
a3i32 a3kinematicsSolveInversePartial(const a3_HierarchyState* hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount)
{
	if (hierarchyState && hierarchyState->hierarchy &&
		firstIndex < hierarchyState->hierarchy->numNodes && nodeCount)
	{
		// implement inverse kinematics algorithm: 
		//	- for all nodes starting at first index
		//		- if node is not root (has parent node)
		//			- local matrix = inverse parent object matrix * object matrix
		//		- else
		//			- copy object matrix to local matrix
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: DONE IN CLASS
//-----------------------------------------------------------------------------

		const a3_HierarchyNode* itr = hierarchyState->hierarchy->nodes + firstIndex;
		const a3_HierarchyNode* const end = itr + nodeCount;
		for (; itr < end; ++itr)
		{
			if (itr->parentIndex >= 0)
				a3kinematicsSolveInverseSingle(hierarchyState, itr->index, itr->parentIndex);
			else
				a3kinematicsSolveInverseRoot(hierarchyState, itr->index);
		}
		return (a3i32)(end - itr);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------

void a3kinematicsUpdateHierarchyStateFK(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup)
{
	if (activeHS->hierarchy == baseHS->hierarchy &&
		activeHS->hierarchy == poseGroup->hierarchy)
	{
		// FK pipeline
		//	-> concatenate base pose
		//	-> convert poses to local-space matrices
		//	-> perform recursive FK
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

		a3hierarchyPoseConcat(activeHS->localSpace,	// local: goal to calculate
			activeHS->animPose,						// holds current sample pose
			baseHS->localSpace,						// holds base pose (animPose is all identity poses)
			activeHS->hierarchy->numNodes);
		a3hierarchyPoseConvert(activeHS->localSpace,
			activeHS->hierarchy->numNodes,
			poseGroup->channel,
			poseGroup->order);
		a3kinematicsSolveForward(activeHS);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
}

void a3kinematicsUpdateHierarchyStateIK(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup)
{
	if (activeHS->hierarchy == baseHS->hierarchy &&
		activeHS->hierarchy == poseGroup->hierarchy)
	{
		// IK pipeline
		//	-> perform recursive IK
		//	-> restore local-space matrices to poses
		//	-> deconcatenate base pose
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: DONE IN CLASS
//-----------------------------------------------------------------------------

		a3kinematicsSolveInverse(activeHS);
		a3hierarchyPoseRestore(activeHS->localSpace,
			activeHS->hierarchy->numNodes,
			poseGroup->channel,
			poseGroup->order);
		a3hierarchyPoseDeconcat(
			activeHS->animPose, // result: animation pose
			activeHS->localSpace, // LH Input: Local pose
			baseHS->localSpace, // Subtract base local
			activeHS->hierarchy->numNodes);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
	}
}

void a3kinematicsUpdateHierarchyStateSkin(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS)
{
	if (activeHS->hierarchy == baseHS->hierarchy)
	{
		// FK pipeline extended for skinning and other applications
		//	-> update local-space inverse matrices
		//	-> update object-space inverse matrices
		//	-> update transform from base to current
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

		a3hierarchyStateUpdateLocalInverse(activeHS);
		a3hierarchyStateUpdateObjectInverse(activeHS);
		a3hierarchyStateUpdateObjectBindToCurrent(activeHS, baseHS);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
}


// helper to resolve single-joint IK after solver
static void a3kinematicsResolvePostIK(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup,
	a3ui32 const nodeIndex, a3real4x4 const j2obj)
{
	// post-IK resolution for single affected joint
	//	-> reassign resolved transform to object-space
	//	-> compute object-space inverse matrix
	//	-> compute local-space matrix
	//	-> restore local-space matrix to pose
	//	-> deconcatenate base pose
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// Jerry
	// Reassign resolved transform to obj space
	a3real4x4SetReal4x4(activeHS->objectSpace->hpose_base[nodeIndex].transformMat.m, j2obj); 

	// compute object-space inverse matrix
	a3real4x4GetInverse(activeHS->objectSpaceInv->hpose_base[nodeIndex].transformMat.m, j2obj);

	// compute local-space matrix
	a3real4x4Product(activeHS->localSpace->hpose_base[nodeIndex].transformMat.m,
					 activeHS->objectSpaceInv->hpose_base[activeHS->hierarchy->nodes[nodeIndex].parentIndex].transformMat.m,
					 baseHS->objectSpace->hpose_base[nodeIndex].transformMat.m);

	// restore ls matrix to pose - a3spatialPoseRestore();
	a3spatialPoseRestore(poseGroup->hpose->hpose_base + nodeIndex, poseGroup->channel[nodeIndex], poseGroup->order[nodeIndex]);

	// Deconcatenate base pose - a3spatialPoseDeconcat()
	a3spatialPoseDeconcat(activeHS->animPose->hpose_base + nodeIndex, activeHS->localSpace->hpose_base + nodeIndex, baseHS->localSpace->hpose_base + nodeIndex);
//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}

void a3kinematicsUpdateLookAtIK(a3_HierarchyState const* sceneGraphState,
	a3_HierarchyState* activeHS, a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup,
	a3ui32 const sceneGraphIndex_hierarchyObj, a3ui32 const sceneGraphIndex_effector, 
	a3ui32 const hierarchyObjIndex_affected, a3_Basis const basis_hierarchyObj, a3_Basis const basis_affected)
{
	// Jerry being too lazy to type out class names instead of typing even though hes writing out this comment at this exact moment...
	a3mat3 m_hierarchyObj, m_affected;
	if (!a3basisToMat3(m_hierarchyObj.m, basis_hierarchyObj))
		return;
	if (!a3basisToMat3(m_affected.m, basis_affected))
		return;

	if ((!sceneGraphState || !activeHS || !baseHS || !poseGroup) ||
		(activeHS->hierarchy != baseHS->hierarchy) ||
		(activeHS->hierarchy != poseGroup->hierarchy))
		return;
	
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------
	// JERRY

	// FIRST STEP:
	// transform everything into the space of the skeleton/hierarchy
	// -> look at target

	a3real4x4* hierachyRig = &sceneGraphState->localSpace->hpose_base[sceneGraphIndex_hierarchyObj].transformMat.m;

	// transforms scene based on the matrixactiveHS->objectSpace->hpose_base[hierarchyObjIndex_affected].transformMat.v3.x
	a3vec4 hierachyEffector, jDiff;
	// taking direction vector from RUDE
	a3real4ProductTransform(hierachyEffector.v, &sceneGraphState->localSpaceInv->hpose_base[sceneGraphIndex_effector].transformMat.v3.x, *hierachyRig);

	// MIDDLE STEP:
	// solver: build an orthonormal basis -> joint-to-object
	//  1. Direction basis = target - joint pos
	//  2. Side basis = known up x direction basis (cross product)
	//	3. Up basis = direction basis x side basis (cross product)
	//  4. Normalize all (1 and 2)

	// calculate difference between effector and neck joint
	a3real4Diff(&jDiff.x,
		&activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected].transformMat.v3.x, // direction matrix of affected
		hierachyEffector.v);

	a3vec3 yOne = { 0, 1, 0 };
	a3mat4 lookAt;

	// direction basis [r t; D 1]
	// direction basis is the vector from the target to the objects position and it requires a normalized
	//a3real3Cross(&basis.v2.x, &yOne.y, &jDiff.v);
	a3real4Set(&lookAt.v2.x, jDiff.x, jDiff.y, jDiff.z, 0);
	// Normalizing the direction basis
	a3real4Normalize(&lookAt.v2.x);

	// right basis [R t ; d 1]
	a3real3Cross(&lookAt.v0.x, &yOne.y, &jDiff.x);
	// Normalizing the up basis
	a3real4Normalize(&lookAt.v0.x);

	// up vector [r T ; d 1] - no need to normalize, others are already normalized
	a3real3Cross(&lookAt.v1.x, &lookAt.v0.x, &lookAt.v2.x);

	//a3real4x4Product(&lookAt.m, m_affected.m, &lookAt.m);

	//a3real4Set(&lookAt.v3.x, 
	//	activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected].transformMat.v3.x,
	//	activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected].transformMat.v3.y,
	//	activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected].transformMat.v3.z,
	//	1);

	// LAST STEP:
	// resolve every affected joint: 
	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected, lookAt.m);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}

/* Jerry Notes
	sceneGraphIndex_hierarchyObj - indices, space u wanna be in
	sceneGraphIndex_effector_end - 
	sceneGraphIndex_constraint - pull vector constraint

	hierarchyObjIndex_affected_end -  
	hierarchyObjIndex_affected_hinge - knee/elbow
	hierarchyObjIndex_affected_base - shoulder/hip

	basis_hierarchyObj - describing how node is oriented in world space
*/

void a3kinematicsUpdateLimbIK(a3_HierarchyState const* sceneGraphState,
	a3_HierarchyState* activeHS, a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup,
	a3ui32 const sceneGraphIndex_hierarchyObj, a3ui32 const sceneGraphIndex_effector_end, a3ui32 const sceneGraphIndex_constraint,
	a3ui32 const hierarchyObjIndex_affected_end, a3ui32 const hierarchyObjIndex_affected_hinge, a3ui32 const hierarchyObjIndex_affected_base,
	a3_Basis const basis_hierarchyObj, a3_Basis const basis_affected_end, a3_Basis const basis_affected_hinge, a3_Basis const basis_affected_base)
{
	a3mat3 m_hierarchyObj, m_affected_end, m_affected_hinge, m_affected_base;
	if (!a3basisToMat3(m_hierarchyObj.m, basis_hierarchyObj))
		return;
	if (!a3basisToMat3(m_affected_end.m, basis_affected_end))
		return;
	if (!a3basisToMat3(m_affected_hinge.m, basis_affected_hinge))
		return;
	if (!a3basisToMat3(m_affected_base.m, basis_affected_base))
		return;
	
	if ((!sceneGraphState || !activeHS || !baseHS || !poseGroup) ||
		(activeHS->hierarchy != baseHS->hierarchy) ||
		(activeHS->hierarchy != poseGroup->hierarchy))
		return;
	
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

	a3real4x4* hierachyRig = &sceneGraphState->localSpace->hpose_base[sceneGraphIndex_hierarchyObj].transformMat.m;

	// transforms scene based on the matrixactiveHS->objectSpace->hpose_base[hierarchyObjIndex_affected].transformMat.v3.x
	a3vec4 hierachyEffector;
	// taking direction vector from RUDE
	a3real4ProductTransform(hierachyEffector.v, &sceneGraphState->localSpaceInv->hpose_base[sceneGraphIndex_effector_end].transformMat.v3.x, *hierachyRig);

	activeHS->hpose->hpose_base[1].translate.x = hierachyEffector.x;

	// FIRST STEP:
	// transform everything into the space of the skeleton/hierarchy
	//  -> wrist effector
	//  -> pull vector constraint

	// direction basis [r t; D 1]
	// direction basis is the vector from the target to the objects position and it requires a normalized
	//a3real3Cross(&basis.v2.x, &yOne.y, &jDiff.v);
	// Normalizing the direction basis
	


	// MAIN STEP:
	// Solve joint-to-object for end, hinge and base
	//	-> end pos*
	//	-> hinge pos*
	// 1. Base joint to end effector (vec) and distance 
	// 2. Base joint to pull vector constraint 
	// 3. plane normal = (base to pull) x (base to end)
	// 4. geometric (heron's formula) or algebraic (law of cosines) 
	//	-> solves elbow pos 
		// https://champlain.instructure.com/courses/2477446/files/362873629?module_item_id=117715617
	// 5. "look at" solves shoulderand elbow rotation as they need to be rebased


	// LAST STEP:
	// resolve every affected joint: 
	//	-> Work from root to leaf
	//a3kinematicsResolvePostIK
	//a3kinematicsResolvePostIK
	//a3kinematicsResolvePostIK


//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}


//-----------------------------------------------------------------------------
