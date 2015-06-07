package com.as3toolkit.ui {
	
	import flash.display.DisplayObject;
	
	import flash.events.EventDispatcher;
	import flash.events.MouseEvent;
	
	public class Mouser extends EventDispatcher{
		
		private var mouseButtonDown:Boolean = false;
		private var elapsed:Date;
		
		public function Mouser(aDO:DisplayObject):void {
			aDO.stage.addEventListener(MouseEvent.MOUSE_DOWN, onMD);
			aDO.stage.addEventListener(MouseEvent.MOUSE_UP, onMU);
		}
		
		private function onMD(e:MouseEvent):void {
			if(mouseButtonDown != true){
				mouseButtonDown = true;
				elapsed = new Date();
				dispatchEvent(new MouserEvent(MouserEvent.BUTTON_PRESS, e));
			}
		}
		
		private function onMU(e:MouseEvent):void {
			mouseButtonDown = false;
			dispatchEvent(new MouserEvent(MouserEvent.BUTTON_RELEASE, e, holdDuration()));
			elapsed = null;
		}
		
		public function mouseIsDown():Boolean {
			return mouseButtonDown;
		}
		
		public function holdDuration():Number {
			var now:Date = new Date();
			var then:Date = elapsed;
			if (then) {
				return now.getTime() - elapsed.getTime();
			} else {
				return 0;
			}
		}
		
	}
	
}