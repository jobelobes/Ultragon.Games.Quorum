package quorum.xbox.events {
	
	import flash.events.Event;
	
	public class XBox360ConnectEvent extends Event {
		
		public static const CONNECT:String = "controllerFound";
		public static const DISCONNECT:String = "controllerLost";
		
		public var controllerIndex:int;
		
		public function XBox360ConnectEvent(atype:String, aIndex:int):void {
			super(atype);
			controllerIndex = aIndex;
		}
		
	}
	
}