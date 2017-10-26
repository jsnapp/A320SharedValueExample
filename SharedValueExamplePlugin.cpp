/*
* SharedValueExamplePlugin.c
*
* This plugin shows how to connect to the FF A320 shared value interface and get values (and could set values as well) 
*
*/

#pragma warning(disable: 4996)

//Not all of these actually need to be included for this example...
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMDataAccess.h"
#include "XPLMPlugin.h"
#include "XPLMUtilities.h"
#include "XPLMProcessing.h"
#include "XPLMScenery.h"
#include "XPLMMenus.h"
#include "XPLMGraphics.h"
#include "XPLMPlanes.h"
#include "XPLMDataAccess.h"
#include "XPLMNavigation.h"
#include "XPWidgets.h"
#include "XPStandardWidgets.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <math.h>

//This is critical to include!
#include "SharedValue.h"


#define XPLM200 = 1;  // This example requires SDK2.0

//If you create datarefs based on shared values, you are going to want
//to register them with Lee Baker's datarefeditor...
//(not shown in this example at present)
#define MSG_ADD_DATAREF 0x01000000           //  Add dataref to DRE message

XPLMPluginID ffPluginID = XPLM_NO_PLUGIN_ID;
SharedValuesInterface ffAPI;

int ffAPIdataversion = 0;
bool printed_to_log = FALSE;

void* tag = "a320sharedflightdeck";

char gearStateString[1000];

double last_step;

float PluginCustomFlightLoopCallback(float elapsedMe, float elapsedSim, int counter, void * refcon); // Declare callback for handling networking data each flight loop

void ffAPIUpdateCallback(double step, void *tag);

/*
* XPluginStart
*
* Our start routine registers our window and does any other initialization we
* must do.
*
*/
PLUGIN_API int XPluginStart(
	char *		outName,
	char *		outSig,
	char *		outDesc)
{


	XPLMMenuID	id;
	int			item;

	/* First we must fill in the passed in buffers to describe our
	* plugin to the plugin-system. */

	strcpy(outName, "A320 Shared Value Example");
	strcpy(outSig, "cockpitconfusion.dummyplugin");
	strcpy(outDesc, "Plugin to get home cockpit people confused and thinking.");

	
	XPLMRegisterFlightLoopCallback(PluginCustomFlightLoopCallback, 1, NULL); // This will get called during each flight loop and we will handle network events

	return 1;
}


void ffAPIUpdateCallback(double step, void *tag) {

	last_step = step;
	//sprintf(sh_string, "ffAPI step time: %f\n", step);

	if (!printed_to_log) {
		printed_to_log = TRUE;
	
		unsigned int valuesCount = ffAPI.ValuesCount();

		/*
		SharedValuesCountProc ValuesCount; // get count off all values (including deleted)
		SharedValueIdByIndexProc ValueIdByIndex; // get value id by it's index (0 up to ValuesCount), or -1 if not exists or removed
		SharedValueIdByNameProc ValueIdByName; // get value id by it's name, or -1 if not exists or removed
		SharedValueNameProc ValueName; // get value name
		SharedValueDescProc ValueDesc; // get value description
		SharedValueTypeProc ValueType; // get value type, one of Value_Type_
		SharedValueFlagsProc ValueFlags; // get value flags, OR Value_Flag_
		SharedValueUnitsProc ValueUnits; // get value units, OR Value_Unit_
		SharedValueParentProc ValueParent; // get id of the parent object value
		*/

		int valueID = -1;
		char stringBuffer[1000];


		XPLMDebugString("=============\n");
		sprintf(stringBuffer, "COUNT OF VALUES: %d\n", valuesCount);
		XPLMDebugString(stringBuffer);

		unsigned int ii = 0;
		for (ii = 0; ii < valuesCount; ii++) {

			char *valueName, *valueDescription;

			//Get value id by index and get Value Name...			
			valueID = ffAPI.ValueIdByIndex(ii);
			valueName = (char *) ffAPI.ValueName(valueID);
			//sprintf(stringBuffer, "ID: %d\n", valueID);

			//XPLMDebugString("--------\n");
			//XPLMDebugString(stringValue);
			//XPLMDebugString("\n");
			//XPLMDebugString(stringBuffer);

			//Find value description and print it...
			valueDescription = (char *) ffAPI.ValueDesc(valueID);
			//XPLMDebugString(stringValue);
			//XPLMDebugString("\n");

			unsigned int valueType = ffAPI.ValueType(valueID);
			unsigned int valueFlag = ffAPI.ValueFlags(valueID);

			//Find parent ID and make string to print the parent id...
			int parentValueID = ffAPI.ValueParent(valueID);
			//sprintf(stringBuffer, "PARENT ID: %d\n", valueID);
			//XPLMDebugString(stringBuffer);

			char *valueTypeString;

			if (valueType == Value_Type_Deleted) {
				valueTypeString = "Value_Type_Deleted";
			} else if (valueType == Value_Type_Object) {
				valueTypeString = "Value_Type_Object";
			} else if (valueType == Value_Type_sint8) {
				valueTypeString = "Value_Type_sint8";
			} else if (valueType == Value_Type_uint8) {
				valueTypeString = "Value_Type_uint8";
			} else if (valueType == Value_Type_sint16) {
				valueTypeString = "Value_Type_sint16";
			} else if (valueType == Value_Type_uint16) {
				valueTypeString = "Value_Type_uint16";
			} else if (valueType == Value_Type_sint32) {
				valueTypeString = "Value_Type_sint32";
			} else if (valueType == Value_Type_uint32) {
				valueTypeString = "Value_Type_uint32";
			} else if (valueType == Value_Type_float32) {
				valueTypeString = "Value_Type_float32";
			} else if (valueType == Value_Type_float64) {
				valueTypeString = "Value_Type_float64";
			} else if (valueType == Value_Type_String) {
				valueTypeString = "Value_Type_String";
			} else if (valueType == Value_Type_Time) {
				valueTypeString = "Value_Type_Time";
			}
			else {
				valueTypeString = "UNKNOWN";
			}
			
				/*
				
				#define Value_Type_Deleted				0
				#define Value_Type_Object				1
				#define Value_Type_sint8				2
				#define Value_Type_uint8				3
				#define Value_Type_sint16				4
				#define Value_Type_uint16				5
				#define Value_Type_sint32				6
				#define Value_Type_uint32				7
				#define Value_Type_float32				8
				#define Value_Type_float64				9
				#define Value_Type_String				10
				#define Value_Type_Time					11
				
				*/


			sprintf(stringBuffer, "%d, %d, %s, %s, %s, %d\n", parentValueID, valueID, valueName, valueDescription, valueTypeString, valueFlag);
			XPLMDebugString(stringBuffer);

		}

		
		

	} else {

		//Landing gear state....
		float gearSwitchPosition, swcontact1_failure, swcontact2_failure;
		unsigned int gearSwitchTarget, swstate1, swstate2;
		
		ffAPI.ValueGet(1696, &swcontact1_failure);
		ffAPI.ValueGet(1697, &swcontact2_failure);


		ffAPI.ValueGet(1698, &gearSwitchPosition);
		ffAPI.ValueGet(1699, &gearSwitchTarget);

		ffAPI.ValueGet(1700, &swstate1);
		ffAPI.ValueGet(1701, &swstate2);

		sprintf(gearStateString, "Gear: pos: %f tgt: %d fail1: %f fail2: %f stat1: %d stat2: %d", gearSwitchPosition, gearSwitchTarget, 
			swcontact1_failure, swcontact2_failure, swstate1, swstate2);

	}
	
	//float swcontact1_failure(1.0), swcontact2_failure(1.0);
	//ffAPI.ValueSet(1696, &swcontact1_failure);
	//ffAPI.ValueSet(1697, &swcontact2_failure);
	

}

float PluginCustomFlightLoopCallback(float elapsedMe, float elapsedSim, int counter, void * refcon)
{
	if (ffPluginID == XPLM_NO_PLUGIN_ID) {
		ffPluginID = XPLMFindPluginBySignature(XPLM_FF_SIGNATURE);
		
		//Return the interval we next want to be called in..
		return -1.0;
	}

	//If the a320 plugin is loaded we can send a message to it and pass the ffAPI to get the
	//reference to the shared value API...
	XPLMSendMessageToPlugin(ffPluginID, XPLM_FF_MSG_GET_SHARED_INTERFACE, &ffAPI);

	//Make sure DataVersion is valid function pointer...
	if (ffAPI.DataVersion != NULL) {
		ffAPIdataversion = ffAPI.DataVersion();
	}

	//Make sure DataAddUpdate is valid function pointer...
	if (ffAPI.DataAddUpdate != NULL) {
		
		//We add a callback that will be called every cycle that the a320 shared values gets updated
		//this is pretty much the same as registering a flight loop callback in x-plane
		//except you can't set an interval, etc.
		//Adds one with a specific tag we can check against when we get the callback....
		ffAPI.DataAddUpdate(&ffAPIUpdateCallback, tag);

		//Don't need to be called again because we will be relying on getting called
		//by the a320 api callback from now on, not x-plane.
		return 0;
	}

	//We keep waiting for the next flight loop callback until the a320 plugin has been loaded
	//and is up and running and ready for us to connect to it.
	return -1.0;
}


/**********************************************/

/*
* XPluginStop
*
* Our cleanup routine deallocates our window.
*
*/
PLUGIN_API void	XPluginStop(void)
{
	ffAPI.DataDelUpdate(&ffAPIUpdateCallback, tag);
	ffPluginID = XPLM_NO_PLUGIN_ID;

}


/*
* XPluginDisable
*
* We do not need to do anything when we are disabled, but we must provide the handler.
*
*/
PLUGIN_API void XPluginDisable(void)
{
}

/*
* XPluginEnable.
*
* We don't do any enable-specific initialization, but we must return 1 to indicate
* that we may be enabled at this time.
*
*/
PLUGIN_API int XPluginEnable(void)
{
	return 1;
}

/*
* XPluginReceiveMessage
*
* We don't have to do anything in our receive message handler, but we must provide one.
*
*/
PLUGIN_API void XPluginReceiveMessage(
	XPLMPluginID	inFromWho,
	int				inMessage,
	void *			inParam)
{
}
