package com.as3toolkit.ui {
	
	import flash.display.DisplayObject;
	
	import flash.events.EventDispatcher;
	import flash.events.KeyboardEvent;
	
	public class Keyboarder extends EventDispatcher{
		
		private var keys:Array;
		private var elapsed:Array;
		
		public function Keyboarder(aDO:DisplayObject):void {
			keys = new Array();
			elapsed = new Array();
			aDO.stage.addEventListener(KeyboardEvent.KEY_DOWN, onKD);
			aDO.stage.addEventListener(KeyboardEvent.KEY_UP, onKU);
		}
		
		private function onKD(e:KeyboardEvent):void {
			if(keys[e.keyCode] != true){
				keys[e.keyCode] = true;
				elapsed[e.keyCode] = new Date();
				dispatchEvent(new KeyboarderEvent(KeyboarderEvent.FIRST_PRESS, e.keyCode));
			}
		}
		
		private function onKU(e:KeyboardEvent):void {
			keys[e.keyCode] = false;
			dispatchEvent(new KeyboarderEvent(KeyboarderEvent.RELEASE, e.keyCode, holdDuration(e.keyCode)));
			elapsed[e.keyCode] = null;
		}
		
		public function keyIsDown(aKey:Number):Boolean {
			return keys[aKey]?true:false;
		}
		
		public function holdDuration(aKey:Number):Number {
			var now:Date = new Date();
			var then:Date = elapsed[aKey];
			if (then) {
				return now.getTime() - elapsed[aKey].getTime();
			} else {
				return 0;
			}
		}
		
	}
	
}