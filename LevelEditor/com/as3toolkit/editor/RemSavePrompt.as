package com.as3toolkit.editor {
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import flash.text.TextField;
	
	public class RemSavePrompt extends MovieClip {
		
		public function RemSavePrompt(args:Array):void {
			save_btn.gotoAndStop("_up");
			quit_btn.gotoAndStop("_up");
			ok_btn.gotoAndStop("_up");
		}
		
	}
	
}