package com.as3toolkit.ui {
	
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	public class MouserEvent extends Event {
		
		public static const BUTTON_PRESS:String = "buttonJustPressed";
		public static const BUTTON_RELEASE:String = "buttonReleased";
		
		public var me:MouseEvent;
		public var duration:Number;
		
		public function MouserEvent(atype:String, ae:MouseEvent, adur:Number = 0):void {
			super(atype);
			me = ae;
			duration = adur;
		}
		
	}
	
}