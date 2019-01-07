// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class CubePhysicsDemoTarget : TargetRules
{
	public CubePhysicsDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "CubePhysicsDemo" } );
	}
}
