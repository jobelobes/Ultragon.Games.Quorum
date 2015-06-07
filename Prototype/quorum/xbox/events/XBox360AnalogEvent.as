package quorum.xbox.events {
	
	import flash.events.Event;
	
	public class XBox360AnalogEvent extends Event {
		
		public static const ANALOG_MOVE:String = "analogMove";
		
		public var analogIndex:int;
		
		public function XBox360AnalogEvent(atype:String, ai:int):void {
			super(atype);
			analogIndex = ai;
		}
		
	}
	
}