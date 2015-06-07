package com.as3toolkit.ui {
	
	import flash.events.Event;
	
	public class KeyboarderEvent extends Event {
		
		public static const FIRST_PRESS:String = "keyJustPressed";
		public static const RELEASE:String = "keyReleased";
		
		public var keyCode:Number;
		public var duration:Number;
		
		public function KeyboarderEvent(atype:String, akc:Number, adur:Number = 0):void {
			super(atype);
			keyCode = akc;
			duration = adur;
		}
		
	}
	
}