package com.as3toolkit.editor {
	
	import flash.display.MovieClip;
	
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import flash.geom.Point;
	
	import flash.text.TextField;
	
	public class LayerPrompt extends MovieClip {
		
		private var _layer:Layer;
		private var _vis:Boolean = true;
		
		public function LayerPrompt(args:Array):void {
			_layer = args[0];
			label_txt.text = _layer.label_txt.text;
			parallax_txt.text = ""+_layer.parallax;
			visible_btn.gotoAndStop(_layer.visible_btn.currentLabel);
			ok_btn.gotoAndStop("_up");
			
			visible_btn.addEventListener(MouseEvent.CLICK, onToggleVisible);
			addEventListener("OK_CLICK", onOK);
		}
		
		private function onToggleVisible(e:MouseEvent):void {
			_vis = !_vis;
			if(_vis){
				visible_btn.gotoAndStop("green");
			} else {
				visible_btn.gotoAndStop("red");
			}
		}
		
		private function onOK(e:Event):void {
			_layer.label_txt.text = label_txt.text;
			_layer.parallax = Number(parallax_txt.text);
			_layer.setVisible(visible_btn.currentLabel=="green"?true:false);
		}
		
	}
	
}