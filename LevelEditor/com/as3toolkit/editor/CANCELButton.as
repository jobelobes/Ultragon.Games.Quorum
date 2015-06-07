package com.as3toolkit.editor {
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	public class CANCELButton extends MovieClip {
		
		public function CANCELButton():void {
			buttonMode = true;
			addEventListener(MouseEvent.CLICK, onOK);
		}
		
		private function onOK(e:MouseEvent):void {
			dispatchEvent(new Event("CANCEL_CLICK", true, false));
		}
		
	}
	
}