package quorum.xbox.events {
	
	import flash.events.Event;
	
	public class XBox360ButtonEvent extends Event {
		
		public static const BUTTON_PRESS:String = "buttonPress";
		public static const BUTTON_RELEASE:String = "buttonRelease";
		
		public var buttonIndex:int;
		public var durationHeld:Number;
		
		public function XBox360ButtonEvent(atype:String, abi:int, adur:Number = 0):void {
			super(atype);
			buttonIndex = abi;
			durationHeld = adur;
		}
		
	}
	
}