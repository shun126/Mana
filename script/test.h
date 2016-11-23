/*
 * mana (script)
 * Copyright (c) 2003 Shun Moriya <shun@mnu.sakura.ne.jp>
 */

// actor declarations
actor TestActor
{
	int digitVariableInActor;

	action init;
	action main;
	action talk;
}

actor OtherTestActor
{
	action talk;
}

// structs declarations
struct TestStruct
{
	float realValue;
	int digitValue;
	string stringValue;
	Actor actorValue;
}

// static variables
static allocate 10
{
	float staticRealValue;
}

static
{
	int staticDigitValue;
}

