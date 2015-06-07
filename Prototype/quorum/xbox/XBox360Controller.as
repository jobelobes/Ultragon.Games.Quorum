/*
*  Original Author: Simon Joslin - Oct 2008
*  Rewritten to the taste of Joe Pietruch - Mar 2009
*
*  For use with the XBOX 360 Gamepad Server.exe
*  Server Originally from http://projects.psibaspace.net.au/code.php
*  Server Rewritten by programming genius Mike Ey
*/

package quorum.xbox
{

	import flash.events.EventDispatcher;

	import flash.geom.Point;
	import flash.utils.ByteArray;
	
	import quorum.xbox.events.*;

	public class XBox360Controller extends EventDispatcher
	{

		private var _playerIndex:int;
		private var _connected:Boolean = false;

		private var previous_buttons:Array;
		private var buttons:Array = new Array();
		private var elapsed:Array = new Array();
		public static const A:int = 0;
		public static const B:int = 1;
		public static const X:int = 2;
		public static const Y:int = 3;
		public static const LEFT_BUMPER:int = 4;
		public static const RIGHT_BUMPER:int = 5;
		public static const START:int = 6;
		public static const BACK:int = 7;
		public static const UP:int = 8;
		public static const DOWN:int = 9;
		public static const LEFT:int = 10;
		public static const RIGHT:int = 11;
		public static const BUTTONS_END:int = 12;

		private var previous_analogs:Array;
		private var analogs:Array = new Array();
		public static const LEFT_STICK_X:int = 0;
		public static const LEFT_STICK_Y:int = 1;
		public static const RIGHT_STICK_X:int = 2;
		public static const RIGHT_STICK_Y:int = 3;
		public static const LEFT_TRIGGER:int = 4;
		public static const RIGHT_TRIGGER:int = 5;
		public static const ANALOGS_END:int = 6;

		public static const LEFT_STICK:int = 1;
		public static const RIGHT_STICK:int = 2;

		public function XBox360Controller(playerIndex:int):void
		{
			if (playerIndex < 0)
			{
				throw new Error("quorum.xbox.XBox360Controller() - " + playerIndex + " is not an acceptable player index value.");
			}
			else
			{
				_playerIndex = playerIndex;
				resetControllerValues();
			}
		}

		public function resetControllerValues():void
		{// resets the analogs and buttons to default values
			for (var i:int = LEFT_STICK_X; i < ANALOGS_END; i++)
			{
				analogs[i] = 0.0;
			}

			for (var j:int = A; j < BUTTONS_END; j++)
			{
				buttons[j] = false;
				elapsed[j] = 0;
			}
		}

		public function parseBytes( pInputs:ByteArray ):void
		{// updates the analogs and buttons based on the ByteArray
			/*parseBytes requires a ByteArray containing the bytes
			in the following order:
			
			16 bit short - 0 = disconnected, 1 = connected
			16 bit short - Left stick x value ( range between -255 and 255)
			16 bit short - Left stick y value ( range between -255 and 255)
			16 bit short - Right stick x value ( range between -255 and 255)
			16 bit short - Right stick y value ( range between -255 and 255)
			16 bit short - Left trigger (range between 0 and 255)
			16 bit short - Right trigger (range between 0 and 255)
			16 bit short - A Button - 0 = Released, 1 = Pressed
			16 bit short - B Button - 0 = Released, 1 = Pressed
			16 bit short - X Button - 0 = Released, 1 = Pressed
			16 bit short - Y Button - 0 = Released, 1 = Pressed
			16 bit short - L Button - 0 = Released, 1 = Pressed
			16 bit short - R Button - 0 = Released, 1 = Pressed
			16 bit short - Start Button - 0 = Released, 1 = Pressed
			16 bit short - Back Button - 0 = Released, 1 = Pressed
			16 bit short - Up Button - 0 = Released, 1 = Pressed
			16 bit short - Down Button - 0 = Released, 1 = Pressed
			16 bit short - Left Button - 0 = Released, 1 = Pressed
			16 bit short - Right Button - 0 = Released, 1 = Pressed
			
			if the controller is disconnected the state will be reset*/

			var isConnected:Boolean = Boolean(pInputs.readShort());

			if (_connected == false && isConnected == true)
			{
				dispatchEvent(new XBox360ConnectEvent(XBox360ConnectEvent.CONNECT, _playerIndex));
				_connected = true;
			}
			else if (_connected == true && !isConnected)
			{
				dispatchEvent(new XBox360ConnectEvent(XBox360ConnectEvent.DISCONNECT, _playerIndex));
				_connected = false;
			}

		if (isConnected)
		{
			previous_analogs = analogs;

			var oldNum:Number;
			var newNum:Number;

			for (var i:int = LEFT_STICK_X; i < ANALOGS_END; i++)
			{
				oldNum = previous_analogs[i];
				newNum = Number(pInputs.readShort()) / 255.0;
				switch (i)
				{ // negate the y, because the XBoxController is upside down!
					case LEFT_STICK_Y: newNum *= -1; break;
					case RIGHT_STICK_Y: newNum *= -1; break;
				}

				if (newNum != oldNum)
				{
					analogs[i] = newNum;
					
					dispatchEvent(new XBox360AnalogEvent(XBox360AnalogEvent.ANALOG_MOVE, i));
				}
			}

			previous_buttons = buttons;

			var oldBool:Boolean;
			var newBool:Boolean;

			for (var j:int = A; j < BUTTONS_END; j++)
			{
				oldBool = previous_buttons[j];
				newBool = Boolean(pInputs.readShort());
				if (newBool != oldBool)
				{
					buttons[j] = newBool;

					if (newBool)
					{
						elapsed[j] = new Date();
						dispatchEvent(new XBox360ButtonEvent(XBox360ButtonEvent.BUTTON_PRESS, j));
					}
					else
					{
						dispatchEvent(new XBox360ButtonEvent(XBox360ButtonEvent.BUTTON_RELEASE, j, holdDuration(j)));
						elapsed[j] = 0;
					}
				}
			}
		}
	}//parseBytes

	public function get PlayerIndex():int
	{
		return _playerIndex;
	}
	public function get IsConnected():Boolean
	{
		return _connected;
	}

	public function isDown(buttonIndex:int):Boolean
	{// tells us whether the button is up or down
		return buttons[buttonIndex];
	}
	
	public function buttonStringEquivalent(buttonIndex:int):String
	{// tells us the string name of the button
		switch (buttonIndex)
		{
			case A: return "A"; break;
			case B: return "B"; break;
			case X: return "X"; break;
			case Y: return "Y"; break;
			case START: return "START"; break;
			case BACK: return "BACK"; break;
			case LEFT_BUMPER: return "LEFT_BUMPER"; break;
			case RIGHT_BUMPER: return "RIGHT_BUMPER"; break;
			case UP: return "UP"; break;
			case DOWN: return "DOWN"; break;
			case LEFT: return "LEFT"; break;
			case RIGHT: return "RIGHT"; break;
		}
		return "UNRECOGNIZED";
	}
	
	public function analogStringEquivalent(analogIndex:int):String
	{// tells us the string name of the button
		switch (analogIndex)
		{
			case LEFT_STICK_X: return "LEFT_STICK_X";
			case LEFT_STICK_Y: return "LEFT_STICK_Y";
			case RIGHT_STICK_X: return "RIGHT_STICK_X";
			case RIGHT_STICK_Y: return "RIGHT_STICK_Y";
			case LEFT_TRIGGER: return "LEFT_TRIGGER";
			case RIGHT_TRIGGER: return "RIGHT_TRIGGER";
		}
		return "UNRECOGNIZED";
	}

	public function holdDuration(aButton:int):Number
	{// tells us how long the button has been pressed
		var then:Date = elapsed[aButton];
		if (then)
		{
			var now:Date = new Date();
			return now.getTime() - then.getTime();
		}
		else
		{
			return 0;
		}
	}

	public function analogPosition(analogIndex:int):Number
	{// returns the numerical position of a stick or trigger
		return analogs[analogIndex];
	}

	public function stickAsPoint(stickIndex:int):Point
	{// returns the numerical position of a stick as a Point object
		if (stickIndex <= 1 || stickIndex == 10)
		{
			return new Point(analogs[LEFT_STICK_X], analogs[LEFT_STICK_Y]);
		}
		else if (stickIndex >= 2)
		{
			return new Point(analogs[RIGHT_STICK_X], analogs[RIGHT_STICK_Y]);
		}
		else
		{
			return new Point();
		}
	}


}//class
}//package