package com.as3toolkit.editor {
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	public class OKButton extends MovieClip {
		
		public function OKButton():void {
			buttonMode = true;
			addEventListener(MouseEvent.CLICK, onOK);
		}
		
		private function onOK(e:MouseEvent):void {
			dispatchEvent(new Event("OK_CLICK", true, false));
		}
		
	}
	
}