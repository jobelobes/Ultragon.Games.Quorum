package com.as3toolkit.editor {
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	public class QUITButton extends MovieClip {
		
		public function QUITButton():void {
			buttonMode = true;
			addEventListener(MouseEvent.CLICK, onOK);
		}
		
		private function onOK(e:MouseEvent):void {
			dispatchEvent(new Event("QUIT_CLICK", true, false));
		}
		
	}
	
}