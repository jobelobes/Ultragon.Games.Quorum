package com.as3toolkit.editor {
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	public class SAVEButton extends MovieClip {
		
		public function SAVEButton():void {
			buttonMode = true;
			addEventListener(MouseEvent.CLICK, onOK);
		}
		
		private function onOK(e:MouseEvent):void {
			dispatchEvent(new Event("SAVE_CLICK", true, false));
		}
		
	}
	
}