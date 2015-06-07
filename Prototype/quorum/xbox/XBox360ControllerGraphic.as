package quorum.xbox {
	
	import flash.display.MovieClip;
	
	import quorum.xbox.events.*;
	
	public class XBox360ControllerGraphic extends MovieClip {
	
		private var playerIndex:int;
		
		private var gamepadManager:XBox360Manager;
		private var controller:XBox360Controller;
		
		private var leftstick_init_x;
		private var leftstick_init_y;
		private var rightstick_init_x;
		private var rightstick_init_y;
		private var lefttrigger_init;
		private var righttrigger_init;
		
		public function XBox360ControllerGraphic (aman:XBox360Manager, aIndex:int):void
		{// set up some stuff!
			gamepadManager = aman;
			playerIndex = aIndex;
			controller = gamepadManager.getController(playerIndex);
			
			leftstick_init_x = LeftStick.x;
			leftstick_init_y = LeftStick.y;
			rightstick_init_x = RightStick.x;
			rightstick_init_y = RightStick.y;
			lefttrigger_init = LeftTrigger.y;
			righttrigger_init = RightTrigger.y;
			
			controller.addEventListener(XBox360ConnectEvent.CONNECT, onControllerConnected);
			controller.addEventListener(XBox360ConnectEvent.DISCONNECT, onControllerDisconnected);
			
			controller.addEventListener(XBox360ButtonEvent.BUTTON_PRESS, onButtonPress);
			controller.addEventListener(XBox360ButtonEvent.BUTTON_RELEASE, onButtonRelease);
			controller.addEventListener(XBox360AnalogEvent.ANALOG_MOVE, onAnalogMove);
			
			for(var i:int = 0; i < XBox360Controller.BUTTONS_END; i++)
			{// reset the controller graphics
				MovieClip(this[controller.buttonStringEquivalent(i)]).visible = false;
				alpha=.5;
			}
		}
		
		private function onControllerConnected(e:XBox360ConnectEvent):void
		{// fade in on connect
			alpha=1;
		}
		
		private function onControllerDisconnected(e:XBox360ConnectEvent):void
		{// fade out on disconnect
			alpha=.5;
		}
				
		private function onButtonPress(e:XBox360ButtonEvent):void
		{// updates the button graphics for the controller
			MovieClip(this[controller.buttonStringEquivalent(e.buttonIndex)]).visible = true;
		}
		
		private function onButtonRelease(e:XBox360ButtonEvent):void
		{// updates the button graphics for the controller
			MovieClip(this[controller.buttonStringEquivalent(e.buttonIndex)]).visible = false;
		}
		
		private function onAnalogMove(e:XBox360AnalogEvent):void
		{// updates the analog graphics for the controller
			switch (e.analogIndex) {
				case XBox360Controller.LEFT_STICK_X:
				case XBox360Controller.LEFT_STICK_Y:
					LeftStick.x = leftstick_init_x + (controller.analogPosition(XBox360Controller.LEFT_STICK_X) * 10);
					// - y because flash has the opposite y direction to xna
					LeftStick.y = leftstick_init_y - (controller.analogPosition(XBox360Controller.LEFT_STICK_Y) * 10); 
					break;
				case XBox360Controller.RIGHT_STICK_X:
				case XBox360Controller.RIGHT_STICK_Y:
					RightStick.x = rightstick_init_x + (controller.analogPosition(XBox360Controller.RIGHT_STICK_X) * 10);
					// - y because flash has the opposite y direction to xna
					RightStick.y = rightstick_init_y - (controller.analogPosition(XBox360Controller.RIGHT_STICK_Y) * 10);
					break;
				case XBox360Controller.LEFT_TRIGGER:
					LeftTrigger.y = lefttrigger_init + (controller.analogPosition(XBox360Controller.LEFT_TRIGGER) * 30);
					break;
				case XBox360Controller.RIGHT_TRIGGER:
					RightTrigger.y = righttrigger_init + (controller.analogPosition(XBox360Controller.RIGHT_TRIGGER) * 30);
					break;			
			}
		}
	}	
}
