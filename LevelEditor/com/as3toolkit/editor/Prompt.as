package com.as3toolkit.editor {
	
	import flash.display.MovieClip;
	
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import flash.utils.getDefinitionByName;
	
	public class Prompt extends MovieClip {
		
		private var _currentPrompt:MovieClip;
		
		private var _editor:Editor;
		
		public function Prompt(editor:Editor):void {
			_editor = editor;
			addEventListener(MouseEvent.CLICK, onC);
			addEventListener("OK_CLICK", onOK);
			addEventListener("CANCEL_CLICK", onCancel);
			addEventListener("SAVE_CLICK", onSAVE);
			addEventListener("QUIT_CLICK", onQUIT);
		}
		
		private function onC(e:MouseEvent):void {
			// prevent anything else from happening!
		}
		
		private function onCancel(e:Event):void {
			_editor.abort();
			kill();
		}
		
		private function onOK(e:Event):void {
			kill();
		}
		
		private function onSAVE(e:Event):void {
			_editor.saveXML(null);
			_editor.abort();
			kill();
		}
		
		private function onQUIT(e:Event):void {
			this.stage.nativeWindow.close();
			kill();
		}
		
		public function display(type:String, args:Array):void {
			var cr:Class = getDefinitionByName(type) as Class;
			_currentPrompt = new cr(args);
			addChild(_currentPrompt);
		}
		
		private function kill():void {
			parent.removeChild(this);
			if(contains(_currentPrompt)){
				removeChild(_currentPrompt);
			}
			_currentPrompt = null;
		}
		
	}
	
}