package com.as3toolkit.editor {
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import flash.text.TextField;
	
	public class NotifyPrompt extends MovieClip {
		
		public function NotifyPrompt(args:Array):void {
			ok_btn.gotoAndStop("_up");
			title_txt.text = args[0];
			message_txt.text = args[1];
		}
		
	}
	
}